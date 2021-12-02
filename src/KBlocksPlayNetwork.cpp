/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#include "KBlocksPlayNetwork.h"

#ifdef Q_OS_WIN
#include <windows.h>
#else
#include <unistd.h>
#endif

KBlocksPlayNetwork::KBlocksPlayNetwork(int capacity, const string &serverIP, int localPort)
{
    mServerIP = serverIP;
    mLocalPort = localPort;

    mRunning = false;

    mPlayerCount = 0;
    mMaxCapacity = capacity;

    maPlayerList = new KBlocksNetPlayer*[mMaxCapacity];

    mpGameLogic = new KBlocksGameLogic(mMaxCapacity);
    mpGameLogic->setGameSeed(0);
    mpGameLogic->setGamePunish(false);
    mpGameLogic->setGameStandbyMode(true);
    mpGameLogic->setInitInterval(0);
    mpGameLogic->setLevelUpInterval(0);
}

KBlocksPlayNetwork::~KBlocksPlayNetwork()
{
    mpGameLogic->stopGame();
    delete mpGameLogic;

    delete [] maPlayerList;
}

bool KBlocksPlayNetwork::addGamePlayer(GamePlayerInterface *p)
{
    if (mPlayerCount == mMaxCapacity) {
        return false;
    }
    maPlayerList[mPlayerCount] = new KBlocksNetPlayer(p, mServerIP, mLocalPort + mPlayerCount);
    mPlayerCount++;
    return true;
}

void KBlocksPlayNetwork::clearGamePlayer()
{
    for (int i = 0; i < mPlayerCount; i++) {
        delete maPlayerList[i];
        maPlayerList[i] = nullptr;
    }
    mPlayerCount = 0;
}

void KBlocksPlayNetwork::startGame()
{
    mpGameLogic->startGame(mPlayerCount);
    for (int i = 0; i < mPlayerCount; i++) {
        mpGameLogic->getSingleGame(i)->stopGame();
        maPlayerList[i]->joinGame(i);
    }
    for (int i = 0; i < mPlayerCount; i++) {
        maPlayerList[i]->startGame(mpGameLogic->getSingleGame(i));
    }
}

void KBlocksPlayNetwork::stopGame()
{
    for (int i = 0; i < mPlayerCount; i++) {
        maPlayerList[i]->stopGame();
        maPlayerList[i]->quitGame();
    }
    mpGameLogic->stopGame();
}

int KBlocksPlayNetwork::execute()
{
    mRunning = true;

    while (mRunning) {
        for (int i = 0; i < mPlayerCount; i++) {
            if (!maPlayerList[i]->execute()) {
                mRunning = false;
                break;
            }
        }
#ifndef Q_OS_WIN
        usleep(100000);
#else
        Sleep(100);
#endif
    }

    return 0;
}

void KBlocksPlayNetwork::cancelExecute()
{
    mRunning = false;
}
