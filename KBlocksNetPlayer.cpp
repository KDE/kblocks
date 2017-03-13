/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksNetPlayer.h"

#include "AI/KBlocksAILog.h"

KBlocksNetPlayer::KBlocksNetPlayer(GamePlayerInterface *player, const string &serverIP, int localPort)
{
    mpNetClient = new KBlocksNetClient(serverIP.c_str(), localPort);

    mpPlayer = player;
    mpGame = nullptr;

    mSendLength = 0;
    mActionList.clear();
}

KBlocksNetPlayer::~KBlocksNetPlayer()
{
    delete mpNetClient;
}

void KBlocksNetPlayer::joinGame(int gameIndex)
{
    int tmpByteCount = 0;
    char tmpByteData[256];
    string tmpName = mpPlayer->getName();

    tmpByteData[tmpByteCount++] = '|';
    tmpByteData[tmpByteCount++] = 'a';
    tmpByteData[tmpByteCount++] = 'p';
    tmpByteData[tmpByteCount++] = '|';
    tmpByteData[tmpByteCount++] = (char)gameIndex + '0';
    tmpByteData[tmpByteCount++] = '|';
    for (size_t i = 0; i < tmpName.length(); ++i) {
        tmpByteData[tmpByteCount++] = (char)tmpName[i];
    }
    tmpByteData[tmpByteCount++] = 0;

    mpNetClient->sendData(tmpByteCount, tmpByteData);
}

void KBlocksNetPlayer::quitGame()
{
    char tmpByteData[5] = {'|', 'd', 'p', '|', '\0'};
    mpNetClient->sendData(5, tmpByteData);
}

void KBlocksNetPlayer::startGame(KBlocksSingleGame *p)
{
    char tmpByteData[4] = {'|', 's', '|', '\0'};
    mpNetClient->sendData(4, tmpByteData);

    mpGame = p;
    mpPlayer->startGame(mpGame);

    mActionList.clear();
}

void KBlocksNetPlayer::stopGame()
{
    char tmpByteData[4] = {'|', 'c', '|', '\0'};
    mpNetClient->sendData(4, tmpByteData);

    mpPlayer->stopGame();
    mpGame = 0;

    mActionList.clear();
}

bool KBlocksNetPlayer::execute()
{
    bool execResult = true;
    char *tmpByteData = new char[256];

    int ret = mpNetClient->recvData(256, tmpByteData);
    if (ret < 0) {
        // Do nothing...
        //printf("--Nothing received\n");
    } else if (tmpByteData[0] == -1) {
        mSendLength = (unsigned char)tmpByteData[1];
        //printf("--Send Length = %d\n", mSendLength);
    } else if (tmpByteData[0] == 127) {
        execResult = false;
        //printf("--Game Ended\n");
    } else {
        //printf("++Game Updates (%d) of [%d bytes]\n", (int)tmpByteData[0], ret);
        int tmpPieceCount = formIntFromByte(tmpByteData + 13);
        for (int i = 0; i < tmpPieceCount; ++i) {
            mpGame->getPiece(i)->decodeData((unsigned char *)tmpByteData + 17 + i * 4);
        }

        mpGame->getField()->decodeData((unsigned char *)tmpByteData + 18 + tmpPieceCount * 4);

        mActionList.clear();
        mpPlayer->think(&mActionList);

        GamePlayer_ActionList::iterator it;
        int byteCounter = 0;
        char *tmpSendData = new char[256];
        tmpSendData[byteCounter++] = '|';
        tmpSendData[byteCounter++] = 'r';
        tmpSendData[byteCounter++] = 'p';
        tmpSendData[byteCounter++] = '|';
        if (mSendLength == 0) {
            for (it = mActionList.begin(); it != mActionList.end(); ++it) {
                tmpSendData[byteCounter++] = (char) * it + '0';
            }
        } else if (!mActionList.empty()) {
            for (int i = 0; i < mSendLength; i++) {
                tmpSendData[byteCounter++] = (char)mActionList.front() + '0';
                mActionList.pop_front();
                if (mActionList.empty()) {
                    break;
                }
            }
        }
        tmpSendData[byteCounter++] = '|';
        tmpSendData[byteCounter++] = '\0';

        mpNetClient->sendData(byteCounter, tmpSendData);
        //printf("Sending : [%s]\n", tmpSendData);

        delete [] tmpSendData;
    }

    delete [] tmpByteData;

    return execResult;
}

int KBlocksNetPlayer::formIntFromByte(char *data)
{
    int value = 0;
    value += ((int)data[0]) & 0x000000FF;
    value += (((int)data[1]) <<  8) & 0x0000FF00;
    value += (((int)data[2]) << 16) & 0x00FF0000;
    value += (((int)data[3]) << 24) & 0xFF000000;
    return value;
}
