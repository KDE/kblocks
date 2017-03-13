/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksNetServer.h"
#include "GamePlayerInterface.h"
#include "KBlocksDefine.h"
#include "KBlocksSingleGame.h"
#include "KBlocksField.h"
#include "KBlocksPiece.h"

#include <QVector>
#include <QVarLengthArray>

KBlocksNetServer::KBlocksNetServer(KBlocksGameLogic *p, const QString &localIP)
{
    mpGameLogic = p;
    maGameScoreList = nullptr;

    mGameCount = 0;
    mGameStarted = false;
    mWaitForAll = false;
    mTopGameLevel = -1;
    mInitSendLength = 0;
    mLvUpSendLength = 0;

    parseIPString(localIP, &mLocalAddress, &mLocalPort);
    // mRemoteAddress and mRemotePort are only used in recvRemoteData
    // and will be set there.
    mRemotePort = 0;

    mpServerSocket = new QUdpSocket(this);
    mpServerSocket->bind(mLocalAddress, mLocalPort);

    mRunningFlag = false;

    mRecordFileName = string("");
    mRecordFileType = true;
}

KBlocksNetServer::~KBlocksNetServer()
{
    delete mpServerSocket;
}

int KBlocksNetServer::executeGame(int gameCount, bool waitForAll)
{
    mGameCount = gameCount;
    mWaitForAll = waitForAll;
    maGameScoreList = new KBlocksScore*[mGameCount];
    mTopGameLevel = -1;
    for (int i = 0; i < mGameCount; i++) {
        maGameScoreList[i] = new KBlocksScore();
        maGameScoreList[i]->setLevelUpFactor(KBlocksScore_Level_x_Level_x_Factor, 1000);
        maGameScoreList[i]->setScoreUpFactor(10);
    }

    mRunningFlag = true;

    QVector<QByteArray> tmpRecvData;
    QVector<QString> tmpRecvAddr;
    int recvResult = 0;
    while (mRunningFlag) {
        recvRemoteData(&tmpRecvData, &tmpRecvAddr);

        if (tmpRecvData.isEmpty()) {
            recvResult = processGame(-1);
        } else {
            recvResult = 0;
            for (int i = 0; i < tmpRecvData.size(); ++i) {
                int result = parseRemoteData(tmpRecvData[i], tmpRecvAddr[i]);
                if (result != -1) {
                    processGame(result);
                } else {
                    recvResult = 1;
                }
            }

            if ((!mWaitForAll) && (recvResult == 0)) {
                recvResult = processGame(-1);
            }
        }

        if ((recvResult < -1) && mGameStarted) {
            break;
        }
    }

    mRunningFlag = false;

    if (!mRecordFileName.empty()) {
        mpGameLogic->saveRecord(mRecordFileName.c_str(), mRecordFileType);
    }

    sendGameOver();

    printGameResult();

    for (int i = 0; i < mGameCount; i++) {
        delete maGameScoreList[i];
    }
    delete [] maGameScoreList;

    if (recvResult != -2) {
        return 0;
    }

    return recvResult;
}

void KBlocksNetServer::setSendLength(int initLen, int lvUpLen)
{
    mInitSendLength = initLen;
    mLvUpSendLength = lvUpLen;
}

void KBlocksNetServer::setRecordFile(const char *fileName, bool binaryMode)
{
    mRecordFileName = string(fileName);
    mRecordFileType = binaryMode;
}

void KBlocksNetServer::recvRemoteData(QVector<QByteArray> *recvData, QVector<QString> *recvAddr)
{
    if (!mRunningFlag) {
        return;
    }

    recvData->clear();
    recvAddr->clear();

    while (mpServerSocket->hasPendingDatagrams()) {
        QByteArray tmpData;
        tmpData.resize(mpServerSocket->pendingDatagramSize());

        mpServerSocket->readDatagram(tmpData.data(), tmpData.size(), &mRemoteAddress, &mRemotePort);
        QString tmpAddr = formIPString(mRemoteAddress, mRemotePort);

        recvData->append(tmpData);
        recvAddr->append(tmpAddr);

        //printf(" Recv From %s : %s\n", tmpAddr.toStdString().c_str(), tmpData.data());
        //printf("   [%s:%d]\n", mRemoteAddress.toString().toStdString().c_str(), mRemotePort);
    }
}

int KBlocksNetServer::processGame(int gameIndex)
{
    if (gameIndex >= 0) {
        //printf("\tStepping game (%d)...\n", gameIndex);
        mpGameLogic->getSingleGame(gameIndex)->updateGame();
        //printf("\tSending new play data (%d)...\n", gameIndex);
        sendPlayerData(gameIndex);
        return gameIndex;
    } else {
        QVarLengthArray<int, 16> removedLines(mGameCount);
        int activeCount = mpGameLogic->getActiveGameCount();
        if ((activeCount > 1) || (mGameCount == activeCount)) {
            mpGameLogic->updateGame(removedLines.data());
            for (int i = 0; i < mGameCount; i++) {
                if (maGameScoreList[i]->addScore(removedLines[i])) {
                    int tmpLevel = maGameScoreList[i]->getGameLevel();
                    if (mTopGameLevel < tmpLevel) {
                        mpGameLogic->levelUpGame(tmpLevel - mTopGameLevel);
                        mTopGameLevel = tmpLevel;
                        sendPlayerActionLength();
                    }
                }
            }
            if (mWaitForAll) {
                bool allWait = true;
                for (int i = 0; i < mGameCount; i++) {
                    if (mpGameLogic->getSingleGame(i)->isActive()) {
                        allWait = false;
                        break;
                    }
                }
                if (allWait) {
                    mpGameLogic->continueGame();
                }
            }
            return -1;
        } else {
            return -2;
        }
    }
}

void KBlocksNetServer::addPlayerIP(int gameIndex, const QByteArray &data, const QString &addr)
{
    QString playerName = QString(data).mid(6);
    if (mPlayerMapping.find(addr) == mPlayerMapping.end()) {
        mPlayerIPList.append(addr);
    }
    mPlayerMapping[addr] = gameIndex;
    mPlayerName[gameIndex] = playerName;
}

void KBlocksNetServer::delPlayerIP(const QString &addr)
{
    if (mPlayerMapping.find(addr) != mPlayerMapping.end()) {
        mPlayerMapping.remove(addr);
        mPlayerIPList.removeOne(addr);
    }
}

void KBlocksNetServer::sendPlayerActionLength()
{
    QHostAddress tmpIP;
    quint16 tmpPort;
    QByteArray tmpByteData;

    int tmpLength = mInitSendLength - mTopGameLevel * mLvUpSendLength;
    if (mInitSendLength <= 0) {
        return;
    } else {
        if (tmpLength < 1) {
            tmpLength = 1;
        } else if (tmpLength > 255) {
            tmpLength = 255;
        }
    }

    QList<QString>::iterator it;
    for (it = mPlayerIPList.begin(); it != mPlayerIPList.end(); ++it) {
        parseIPString(*it, &tmpIP, &tmpPort);

        tmpByteData.append((char) - 1);
        tmpByteData.append((char)tmpLength);
        tmpByteData.append((char)0);

        mpServerSocket->writeDatagram(tmpByteData, tmpIP, tmpPort);
    }
}

void KBlocksNetServer::sendPlayerData(int gameIndex)
{
    char tmpData[256];
    int tmpByteCount;
    QByteArray tmpByteData;

    QList<QString>::iterator it;
    for (it = mPlayerIPList.begin(); it != mPlayerIPList.end(); ++it) {
        int gameID = mPlayerMapping[*it];

        if (gameIndex == gameID) {
            tmpByteData.clear();

            tmpByteData.append((char)gameID);

            formByteFromInt(maGameScoreList[gameID]->getScorePoint(), tmpData + 0);
            formByteFromInt(maGameScoreList[gameID]->getLineCount(),  tmpData + 4);
            formByteFromInt(maGameScoreList[gameID]->getGameLevel(),  tmpData + 8);
            tmpByteData.append(tmpData, 12);

            int tmpPieceCount = mpGameLogic->getSingleGame(gameID)->getPieceCount();
            formByteFromInt(tmpPieceCount, tmpData);
            tmpByteData.append(tmpData, 4);

            for (int i = 0; i < tmpPieceCount; ++i) {
                mpGameLogic->getSingleGame(gameID)->getPiece(i)->encodeData((unsigned char *)tmpData + i * 4);
            }
            tmpByteData.append(tmpData, tmpPieceCount * 4);

            tmpByteCount = mpGameLogic->getSingleGame(gameID)->getField()->encodeData((unsigned char *)tmpData);
            tmpByteData.append((char)tmpByteCount);
            tmpByteData.append(tmpData, tmpByteCount);

            formByteFromInt(mpGameLogic->getSingleGame(gameID)->getField()->getModifyID(), tmpData);
            tmpByteData.append(tmpData, 4);

            QHostAddress tmpIP;
            quint16 tmpPort;
            parseIPString(*it, &tmpIP, &tmpPort);
            mpServerSocket->writeDatagram(tmpByteData, tmpIP, tmpPort);

            //printf("  Sending data to %d @ %s\n", gameID, it->toStdString().c_str());
            return;
        }
    }
}

void KBlocksNetServer::sendGameOver()
{
    QHostAddress tmpIP;
    quint16 tmpPort;
    QByteArray tmpByteData;

    QList<QString>::iterator it;
    for (it = mPlayerIPList.begin(); it != mPlayerIPList.end(); ++it) {
        parseIPString(*it, &tmpIP, &tmpPort);

        tmpByteData.append((char)127);
        tmpByteData.append((char) - 1);
        tmpByteData.append((char)0);

        mpServerSocket->writeDatagram(tmpByteData, tmpIP, tmpPort);
    }
}

void KBlocksNetServer::sendGuiData(const QString &addr)
{
    char tmpData[256];
    int tmpByteCount;
    QByteArray tmpByteData;

    if (!mGameStarted) {
        return;
    }

    QHostAddress tmpIP;
    quint16 tmpPort;
    parseIPString(addr, &tmpIP, &tmpPort);

    for (int gameID = 0; gameID < mGameCount; ++gameID) {
        tmpByteData.clear();

        tmpByteData.append((char)gameID);

        formByteFromInt(maGameScoreList[gameID]->getScorePoint(), tmpData + 0);
        formByteFromInt(maGameScoreList[gameID]->getLineCount(),  tmpData + 4);
        formByteFromInt(maGameScoreList[gameID]->getGameLevel(),  tmpData + 8);
        tmpByteData.append(tmpData, 12);

        int tmpPieceCount = mpGameLogic->getSingleGame(gameID)->getPieceCount();
        formByteFromInt(tmpPieceCount, tmpData);
        tmpByteData.append(tmpData, 4);

        for (int i = 0; i < tmpPieceCount; ++i) {
            mpGameLogic->getSingleGame(gameID)->getPiece(i)->encodeData((unsigned char *)tmpData + i * 4);
        }
        tmpByteData.append(tmpData, tmpPieceCount * 4);

        tmpByteCount = mpGameLogic->getSingleGame(gameID)->getField()->encodeData((unsigned char *)tmpData);
        tmpByteData.append((char)tmpByteCount);
        tmpByteData.append(tmpData, tmpByteCount);

        formByteFromInt(mpGameLogic->getSingleGame(gameID)->getField()->getModifyID(), tmpData);
        tmpByteData.append(tmpData, 4);

        mpServerSocket->writeDatagram(tmpByteData, tmpIP, tmpPort);
    }
}

int KBlocksNetServer::parseRemoteData(const QByteArray &data, const QString &addr)
{
    int result = -1;
    QString input = QString(data);

    if (input.contains(QStringLiteral("|ap|"))) {
        addPlayerIP((int)(data[4] - '0'), data, addr);
        //printf("Added player @ %s\n", input.toStdString().c_str());
    } else if (input.contains(QStringLiteral("|dp|"))) {
        delPlayerIP(addr);
        //printf("Deleted player @ %s\n", input.toStdString().c_str());
    } else if (input.contains(QStringLiteral("|rp|"))) {
        result = parsePlayerReply(data, addr);
        //printf("Received Player Data @ %s\n", input.toStdString().c_str());
    } else if (input.contains(QStringLiteral("|rg|"))) {
        sendGuiData(addr);
        //printf("Send Gui Data @ %s\n", input.toStdString().c_str());
    } else if (input.contains(QStringLiteral("|s|"))) {
        if (!mGameStarted) {
            mpGameLogic->startGame(mGameCount);
            mGameStarted = true;
            for (int i = 0; i < mGameCount; i++) {
                sendPlayerData(i);
            }
            //printf("Game started\n");
        }
    } else if (input.contains(QStringLiteral("|c|"))) {
        if (mGameStarted) {
            mGameStarted = false;
            mpGameLogic->stopGame();
            //printf("Game stopped\n");
        }
    } else {
        printf("Error transmission data : %s\n", data.data());
    }

    return result;
}

int KBlocksNetServer::parsePlayerReply(const QByteArray &data, const QString &addr)
{
    if (!mGameStarted) {
        return -1;
    }

    if (mPlayerMapping.find(addr) == mPlayerMapping.end()) {
        return -1;
    }

    int gameID = mPlayerMapping[addr];
    KBlocksSingleGame *mpSingleGame = mpGameLogic->getSingleGame(gameID);
    if (mpSingleGame == 0) {
        return -1;
    }

    int counter = 4;
    while (data[counter] != '|') {
        switch (data[counter] - '0') {
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
            while (mpSingleGame->setCurrentPiece(0, 1, 0)) ;
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
        ++counter;
    }

    return gameID;
}

bool KBlocksNetServer::parseIPString(const QString &input, QHostAddress *ip, quint16 *port)
{
    bool result = false;
    ip->setAddress(input.left(input.indexOf(QStringLiteral(":"))));
    *port = input.midRef(input.indexOf(QStringLiteral(":")) + 1).toUInt(&result);
    return result;
}

QString KBlocksNetServer::formIPString(const QHostAddress &inAddr, quint16 inPort)
{
    QString result = inAddr.toString() + QStringLiteral(":%1").arg(inPort);
    return result;
}

void KBlocksNetServer::formByteFromInt(int value, char *data)
{
    data[0] = value & 0x00FF;
    data[1] = (value >>  8) & 0x00FF;
    data[2] = (value >> 16) & 0x00FF;
    data[3] = (value >> 24) & 0x00FF;
}

void KBlocksNetServer::printGameResult()
{
    printf("-------- Game Report --------\n");
    for (int gameID = 0; gameID < mGameCount; gameID++) {
        QString tmpPlayerName = mPlayerName[gameID];
        printf("Game ID : %d\n", gameID);
        printf("\tPlayer Name : %s\n", tmpPlayerName.toLatin1().constData());
        printf("\tTotal Score : %d\n", maGameScoreList[gameID]->getLineCount());
    }
    printf("-----------------------------\n");
}

