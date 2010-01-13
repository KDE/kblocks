/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksNetPlayer.h"

#include "AI/KBlocksAILog.h"

KBlocksNetPlayer::KBlocksNetPlayer(GamePlayerInterface * player, const string& serverIP, int localPort)
{
    mpNetClient = new KBlocksNetClient(serverIP, localPort);
    mpNetClient->setTimeOut(1000);
    
    mpPlayer = player;
    
    mActionList.clear();
}

KBlocksNetPlayer::~KBlocksNetPlayer()
{
    delete mpNetClient;
}

void KBlocksNetPlayer::joinGame(int gameIndex)
{
    int tmpByteCount = 0;
    unsigned char tmpByteData[256];
    string tmpName = mpPlayer->getName();
    
    tmpByteData[tmpByteCount++] = '|';
    tmpByteData[tmpByteCount++] = 'a';
    tmpByteData[tmpByteCount++] = 'p';
    tmpByteData[tmpByteCount++] = '|';
    tmpByteData[tmpByteCount++] = (unsigned char)gameIndex + '0';
    tmpByteData[tmpByteCount++] = '|';
    for(unsigned int i = 0; i < tmpName.length(); i++)
    {
        tmpByteData[tmpByteCount++] = (unsigned char)tmpName[i];
    }
    tmpByteData[tmpByteCount++] = 0;
    
    mpNetClient->sendData(tmpByteCount, tmpByteData);
}

void KBlocksNetPlayer::quitGame()
{
    unsigned char tmpByteData[5] = {'|', 'd', 'p', '|', '\0'};
    mpNetClient->sendData(5, tmpByteData);
}

void KBlocksNetPlayer::startGame(KBlocksSingleGame * p)
{
    unsigned char tmpByteData[4] = {'|', 's', '|', '\0'};
    mpNetClient->sendData(4, tmpByteData);
    
    mLastModifyID = -1;
    
    mpGame = p;
    mpPlayer->startGame(mpGame);
    
    mActionList.clear();
}

void KBlocksNetPlayer::stopGame()
{
    unsigned char tmpByteData[4] = {'|', 'c', '|', '\0'};
    mpNetClient->sendData(4, tmpByteData);
    
    mpPlayer->stopGame();
    mpGame = 0;
    
    mActionList.clear();
}

bool KBlocksNetPlayer::execute()
{
    bool execResult = true;
    int tmpByteCount;
    unsigned char* tmpByteData = new unsigned char[256];
    
    int ret = mpNetClient->recvData(256, tmpByteData);
    if (ret < 0)
    {
        // Do nothing...
    }
    else if (tmpByteData[0] == 255)
    {
        mSendLength = tmpByteData[1];
    }
    else if (tmpByteData[0] == 128)
    {
        execResult = false;
    }
    else
    {
        mpGame->getPiece(0)->decodeData(tmpByteData + 13);
        mpGame->getPiece(1)->decodeData(tmpByteData + 17);
        
        tmpByteCount = tmpByteData[21];
        mpGame->getField()->decodeData(tmpByteData + 22);
        
        int tmpModifyID = mpGame->getField()->getModifyID();
        if (mActionList.empty() || (mLastModifyID != tmpModifyID))
        {
            mActionList.clear();
            mpPlayer->think(&mActionList);
        }
        
        GamePlayer_ActionList::iterator it;
        int byteCounter = 0;
        unsigned char* tmpSendData = new unsigned char[256];
        tmpSendData[byteCounter++] = '|';
        tmpSendData[byteCounter++] = 'r';
        tmpSendData[byteCounter++] = 'p';
        tmpSendData[byteCounter++] = '|';
        if (mSendLength == 0)
        {
            for(it = mActionList.begin(); it != mActionList.end(); it++)
            {
                tmpSendData[byteCounter++] = (unsigned char)*it + '0';
            }
        }
        else if (!mActionList.empty())
        {
            for(int i = 0; i < mSendLength; i++)
            {
                tmpSendData[byteCounter++] = (unsigned char)mActionList.front() + '0';
                mActionList.pop_front();
                if (mActionList.empty())
                {
                    break;
                }
            }
        }
        tmpSendData[byteCounter++] = '|';
        tmpSendData[byteCounter++] = '\0';
        
        mpNetClient->sendData(byteCounter, tmpSendData);
        
        delete [] tmpSendData;
    }
    
    delete [] tmpByteData;
    
    return execResult;
}

int KBlocksNetPlayer::formIntFromByte(unsigned char * data)
{
    int value = 0;
    value += ((int)data[0]) & 0x000000FF;
    value += (((int)data[1]) <<  8) & 0x0000FF00;
    value += (((int)data[2]) << 16) & 0x00FF0000;
    value += (((int)data[3]) << 24) & 0xFF000000;
    return value;
}
