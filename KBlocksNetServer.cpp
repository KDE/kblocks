/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksNetServer.h"
#include "AI/KBlocksAILog.h"
#include "GamePlayerInterface.h"
#include "KBlocksDefine.h"
#include "KBlocksSingleGame.h"
#include "KBlocksField.h"
#include "KBlocksPiece.h"

KBlocksNetServer::KBlocksNetServer(KBlocksGameLogic * p, const string& localIP)
{
    mpGameLogic = p;
    mGameCount = 0;
    mGameStarted = false;
    
    mInitSendLength = 0;
    mLvUpSendLength = 0;
    
    mLocalIP = localIP;
    mTimeOut = 1000;
    
    string tmpIPAddress;
    int tmpPortNum;
    parseIPString(mLocalIP, &tmpIPAddress, &tmpPortNum);
    
    mServerSocketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if (mServerSocketFD < 0)
    {
        printf("\tSocket error.\n");
        return;
    }
    else
    {
        printf("\tSocket %d started.\n", mServerSocketFD);
    }
    
    struct sockaddr_in tmpLocalAddr;
    int tmpLocalAddrLen;
    tmpLocalAddrLen = sizeof(tmpLocalAddr);
    bzero(&tmpLocalAddr, tmpLocalAddrLen);
    tmpLocalAddr.sin_family = AF_INET;
    tmpLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    tmpLocalAddr.sin_port = htons(tmpPortNum);
    
    if (bind(mServerSocketFD, (struct sockaddr *)&tmpLocalAddr, tmpLocalAddrLen) < 0)
    {
        printf("\tBind error\n");
    }
    
    int r = fcntl(mServerSocketFD, F_GETFL, 0);
    fcntl(mServerSocketFD, F_SETFL, r & ~O_NONBLOCK);
    
    mRunningFlag = true;
    
    mRemoteAddrLen = sizeof(mRemoteAddr);
    bzero(&mRemoteAddr, mRemoteAddrLen);
    mRemoteAddr.sin_family = AF_INET;
    mRemoteAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    mRemoteAddr.sin_port = htons(tmpPortNum);
}

KBlocksNetServer::~KBlocksNetServer()
{
    close(mServerSocketFD);
}

int KBlocksNetServer::exec(int gameCount, bool waitForAll)
{
    mGameCount = gameCount;
    maGameScoreList = new KBlocksScore*[mGameCount]();
    mTopGameLevel = -1;
    for(int i = 0; i < mGameCount; i++)
    {
        maGameScoreList[i] = new KBlocksScore();
        maGameScoreList[i]->setLevelUpFactor(KBlocksScore_Level_x_Level_x_Factor, 1000);
        maGameScoreList[i]->setScoreUpFactor(10);
    }
    
    int recvReturn = 0;
    while(mRunningFlag)
    {
        recvReturn = recvRemoteData();
        if (recvReturn < -1)
        {
            break;
        }
        
        recvReturn = processGame(recvReturn, waitForAll);
        if ((!waitForAll) && (recvReturn != -1))
        {
            recvReturn = processGame(-1, waitForAll);
        }
        
        if ((recvReturn < -1) && mGameStarted)
        {
            break;
        }
    }
    
    sendGameOver();
    
    printGameResult();
    
    for(int i = 0; i < mGameCount; i++)
    {
        delete maGameScoreList[i];
    }
    delete [] maGameScoreList;
    
    return recvReturn;
}

void KBlocksNetServer::setTimeOut(int timeOut)
{
    mTimeOut = timeOut;
}

void KBlocksNetServer::setSendLength(int initLen, int lvUpLen)
{
    mInitSendLength = initLen;
    mLvUpSendLength = lvUpLen;
}

int KBlocksNetServer::recvRemoteData()
{
    int netResult;
    fd_set rd_set;
    char rcv_buf[1024];
    
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = mTimeOut;
    
    do
    {
        FD_ZERO(&rd_set);
        FD_SET(mServerSocketFD, &rd_set);
        
        netResult = select((mServerSocketFD+1), &rd_set, NULL, NULL, &timeout);
        if (netResult < 0)
        {
            printf("Select error\n");
            return -3;
        }
        else if (netResult == 0)
        {
            return -1;
        }
        
        netResult = recvfrom(mServerSocketFD, rcv_buf, sizeof(rcv_buf), 0, (struct sockaddr *)&mRemoteAddr, (socklen_t *)&mRemoteAddrLen);
        if (netResult < 0)
        {
            printf("Recv error\n");
            return -2;
        }
        else
        {
            //printf("Received Data : %s @ %s:%d\n", rcv_buf, inet_ntoa(mRemoteAddr.sin_addr), mRemoteAddr.sin_port);
            return parseRemoteData(rcv_buf);
        }
    } while (timeout.tv_usec > 0);
    
    return -1;
}

int KBlocksNetServer::processGame(int gameIndex, bool waitForAll)
{
    if (gameIndex >= 0)
    {
        //printf("\tStepping game (%d)...\n", gameIndex);
        mpGameLogic->getSingleGame(gameIndex)->updateGame();
        //printf("\tSending new play data (%d)...\n", gameIndex);
        sendPlayerData(gameIndex);
        return gameIndex;
    }
    else
    {
        int removedLines[mGameCount];
        
        if (mpGameLogic->getActiveGameCount() > 0)
        {
            mpGameLogic->updateGame(removedLines);
            
            for(int i = 0; i < mGameCount; i++)
            {
                if (maGameScoreList[i]->addScore(removedLines[i]))
                {
                    int tmpLevel = maGameScoreList[i]->getGameLevel();
                    if (mTopGameLevel < tmpLevel)
                    {
                        mpGameLogic->levelUpGame(tmpLevel - mTopGameLevel);
                        mTopGameLevel = tmpLevel;
                        sendPlayerActionLength();
                    }
                }
            }
            
            if (waitForAll)
            {
                bool allWait = true;
                for(int i = 0; i < mGameCount; i++)
                {
                    if (mpGameLogic->getSingleGame(i)->isActive())
                    {
                        allWait = false;
                        break;
                    }
                }
                if (allWait)
                {
                    mpGameLogic->continueGame();
                }
            }
            
            return -1;
        }
        else
        {
            return -2;
        }
    }
}

void KBlocksNetServer::addPlayerIP(int gameIndex, char * name)
{
    string playerName = string(name);
    string remoteIP = formIPString(mRemoteAddr);
    if (mPlayerMapping.find(remoteIP) == mPlayerMapping.end())
    {
        mPlayerIPList.push_back(remoteIP);
    }
    mPlayerMapping[remoteIP] = gameIndex;
    mPlayerName[gameIndex] = playerName;
}

void KBlocksNetServer::delPlayerIP()
{
    string remoteIP = formIPString(mRemoteAddr);
    if (mPlayerMapping.find(remoteIP) != mPlayerMapping.end())
    {
        mPlayerMapping.erase(remoteIP);
        mPlayerIPList.remove(remoteIP);
    }
}

void KBlocksNetServer::sendPlayerActionLength()
{
    string tmpIP;
    int tmpPort;
    unsigned char tmpByteData[3];
    
    struct sockaddr_in tmpTargetAddr;
    int tmpTargetAddrLen = sizeof(tmpTargetAddr);
    bzero(&tmpTargetAddr, tmpTargetAddrLen);
    tmpTargetAddr.sin_family = AF_INET;
    
    int tmpLength = mInitSendLength - mTopGameLevel * mLvUpSendLength;
    if (mInitSendLength <= 0)
    {
        tmpLength = 0;
    }
    else
    {
        if (tmpLength < 1)
        {
            tmpLength = 1;
        }
        else if (tmpLength > 255)
        {
            tmpLength = 255;
        }
    }
    
    list<string>::iterator it;
    for(it = mPlayerIPList.begin(); it != mPlayerIPList.end(); it++)
    {
        parseIPString(*it, &tmpIP, &tmpPort);
        tmpTargetAddr.sin_addr.s_addr = inet_addr(tmpIP.c_str());
        tmpTargetAddr.sin_port = htons(tmpPort);
        
        tmpByteData[0] = 255;
        tmpByteData[1] = (unsigned char)tmpLength;
        tmpByteData[2] = 0;
        
        sendto(mServerSocketFD, tmpByteData, 3, 0, (struct sockaddr *)&tmpTargetAddr, tmpTargetAddrLen);
    }
}

void KBlocksNetServer::sendPlayerData(int gameIndex)
{
    string tmpIP;
    int tmpPort;
    int tmpByteCount;
    unsigned char tmpByteData[1024];
    
    struct sockaddr_in tmpTargetAddr;
    int tmpTargetAddrLen = sizeof(tmpTargetAddr);
    bzero(&tmpTargetAddr, tmpTargetAddrLen);
    tmpTargetAddr.sin_family = AF_INET;
    
    list<string>::iterator it;
    for(it = mPlayerIPList.begin(); it != mPlayerIPList.end(); it++)
    {
        int gameID = mPlayerMapping[*it];
        
        if (gameIndex == gameID)
        {
            parseIPString(*it, &tmpIP, &tmpPort);
            tmpTargetAddr.sin_addr.s_addr = inet_addr(tmpIP.c_str());
            tmpTargetAddr.sin_port = htons(tmpPort);
            
            tmpByteData[0] = gameID;
            
            formByteFromInt(maGameScoreList[gameID]->getScorePoint(), tmpByteData + 1);
            formByteFromInt(maGameScoreList[gameID]->getLineCount(),  tmpByteData + 5);
            formByteFromInt(maGameScoreList[gameID]->getGameLevel(),  tmpByteData + 9);
            
            mpGameLogic->getSingleGame(gameID)->getPiece(0)->encodeData(tmpByteData + 13);
            mpGameLogic->getSingleGame(gameID)->getPiece(1)->encodeData(tmpByteData + 17);
            
            tmpByteCount = mpGameLogic->getSingleGame(gameID)->getField()->encodeData(tmpByteData + 22);
            tmpByteData[21] = tmpByteCount;
            
            formByteFromInt(mpGameLogic->getSingleGame(gameID)->getField()->getModifyID(), tmpByteData + tmpByteCount + 22);
            
            sendto(mServerSocketFD, tmpByteData, tmpByteCount + 26, 0, (struct sockaddr *)&tmpTargetAddr, tmpTargetAddrLen);
        }
    }
}

void KBlocksNetServer::sendGameOver()
{
    string tmpIP;
    int tmpPort;
    unsigned char tmpByteData[3];
    
    struct sockaddr_in tmpTargetAddr;
    int tmpTargetAddrLen = sizeof(tmpTargetAddr);
    bzero(&tmpTargetAddr, tmpTargetAddrLen);
    tmpTargetAddr.sin_family = AF_INET;
    
    list<string>::iterator it;
    for(it = mPlayerIPList.begin(); it != mPlayerIPList.end(); it++)
    {
        parseIPString(*it, &tmpIP, &tmpPort);
        tmpTargetAddr.sin_addr.s_addr = inet_addr(tmpIP.c_str());
        tmpTargetAddr.sin_port = htons(tmpPort);
        
        tmpByteData[0] = 128;
        tmpByteData[1] = 255;
        tmpByteData[2] = 0;
        
        sendto(mServerSocketFD, tmpByteData, 3, 0, (struct sockaddr *)&tmpTargetAddr, tmpTargetAddrLen);
    }
}

void KBlocksNetServer::sendGuiData()
{
    int tmpByteCount;
    unsigned char tmpByteData[1024];
    
    if (!mGameStarted)
    {
        return;
    }
    
    for(int gameID = 0; gameID < mGameCount; gameID++)
    {
        tmpByteData[0] = gameID;
        
        formByteFromInt(maGameScoreList[gameID]->getScorePoint(), tmpByteData + 1);
        formByteFromInt(maGameScoreList[gameID]->getLineCount(),  tmpByteData + 5);
        formByteFromInt(maGameScoreList[gameID]->getGameLevel(),  tmpByteData + 9);
        
        mpGameLogic->getSingleGame(gameID)->getPiece(0)->encodeData(tmpByteData + 13);
        mpGameLogic->getSingleGame(gameID)->getPiece(1)->encodeData(tmpByteData + 17);
        
        tmpByteCount = mpGameLogic->getSingleGame(gameID)->getField()->encodeData(tmpByteData + 22);
        tmpByteData[21] = tmpByteCount;
        
        sendto(mServerSocketFD, tmpByteData, tmpByteCount + 22, 0, (struct sockaddr *)&mRemoteAddr, mRemoteAddrLen);
    }
}

int KBlocksNetServer::parseRemoteData(char * data)
{
    int result = -1;
    string input = string(data);
    
    if (input.find("|ap|") == 0)
    {
        addPlayerIP((int)(data[4] - '0'), data + 6);
        //printf("Added player @ %s\n", input.c_str());
    }
    else if (input.find("|dp|") == 0)
    {
        delPlayerIP();
        //printf("Deleted player @ %s\n", input.c_str());
    }
    else if (input.find("|rp|") == 0)
    {
        result = parsePlayerReply(data);
        //printf("Received Player Data @ %s\n", input.c_str());
    }
    else if (input.find("|rg|") == 0)
    {
        sendGuiData();
        //printf("Send Gui Data @ %s\n", input.c_str());
    }
    else if (input.find("|s|") == 0)
    {
        if (!mGameStarted)
        {
            mpGameLogic->startGame(mGameCount);
            mGameStarted = true;
            for(int i = 0; i < mGameCount; i++)
            {
                sendPlayerData(i);
            }
            //printf("Game started\n");
        }
    }
    else if (input.find("|c|") == 0)
    {
        if (mGameStarted)
        {
            mGameStarted = false;
            mpGameLogic->stopGame();
            //printf("Game stopped\n");
        }
    }
    else
    {
        printf("Error transmission data : %s\n", data);
    }
    
    return result;
}

int KBlocksNetServer::parsePlayerReply(char * data)
{
    if (!mGameStarted)
    {
        return -1;
    }
    
    string remoteIP = formIPString(mRemoteAddr);
    if (mPlayerMapping.find(remoteIP) == mPlayerMapping.end())
    {
        return -1;
    }
    
    int gameID = mPlayerMapping[remoteIP];
    KBlocksSingleGame * mpSingleGame = mpGameLogic->getSingleGame(gameID);
    if (mpSingleGame == 0)
    {
        return -1;
    }
    
    int counter = 4;
    while(data[counter] != '|')
    {
        switch(data[counter] - '0')
        {
            case PlayerAction_Move_Left:
                mpSingleGame->setCurrentPiece(-1, 0, 0);
                break;
            case PlayerAction_Move_Right:
                mpSingleGame->setCurrentPiece(1, 0, 0);
                break;
            case PlayerAction_Move_Down:
                mpSingleGame->setCurrentPiece(0, 1, 0);
                break;
            case PlayerAction_Push_Down:
                while(mpSingleGame->setCurrentPiece(0, 1, 0)) ;
                mpSingleGame->forceUpdateGame();
                break;
            case PlayerAction_Rotate_CW:
                mpSingleGame->setCurrentPiece(0, 0, 1);
                break;
            case PlayerAction_Rotate_CCW:
                mpSingleGame->setCurrentPiece(0, 0, -1);
                break;
            case PlayerAction_None:
            default:
                break;
        }
        counter++;
    }
    
    return gameID;
}

int KBlocksNetServer::parseIPString(const string& input, string * ip, int * port)
{
    *ip = input.substr(0, input.find(":"));
    return sscanf(input.substr(input.find(":") + 1).c_str(), "%d", port);
}

string KBlocksNetServer::formIPString(const sockaddr_in& inAddr)
{
    char tmpData[16];
    sprintf(tmpData, "%s:%d", inet_ntoa(inAddr.sin_addr), ntohs(inAddr.sin_port));
    return string(tmpData);
}

int KBlocksNetServer::getDecIntFromString(const string& input)
{
    char* endptr;
    return strtol(input.c_str(), &endptr, 10);
}

void KBlocksNetServer::formByteFromInt(int value, unsigned char * data)
{
    data[0] = value & 0x00FF;
    data[1] = (value >>  8) & 0x00FF;
    data[2] = (value >> 16) & 0x00FF;
    data[3] = (value >> 24) & 0x00FF;
}

void KBlocksNetServer::printGameResult()
{
    printf("-------- Game Report --------\n");
    for(int gameID = 0; gameID < mGameCount; gameID++)
    {
        string tmpPlayerName = mPlayerName[gameID];
        printf("Game ID : %d\n", gameID);
        printf("\tPlayer Name : %s\n", tmpPlayerName.c_str());
        printf("\tTotal Score : %d\n", maGameScoreList[gameID]->getLineCount());
    }
    printf("-----------------------------\n");
}

