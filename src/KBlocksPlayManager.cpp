/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#include "KBlocksPlayManager.h"

KBlocksPlayManager::KBlocksPlayManager(GameLogicInterface *p, int capacity)
{
    mpGameLogic = p;

    mPlayerCount = 0;
    mMaxCapacity = capacity;

    maPlayerList = new KBlocksSinglePlayer*[mMaxCapacity];
}

KBlocksPlayManager::~KBlocksPlayManager()
{
    delete [] maPlayerList;
}

bool KBlocksPlayManager::addGamePlayer(GamePlayerInterface *p, int thinkInterval, int processInterval)
{
    if (mPlayerCount == mMaxCapacity) {
        return false;
    }
    maPlayerList[mPlayerCount] = new KBlocksSinglePlayer(p, thinkInterval, processInterval);
    mPlayerCount++;
    return true;
}

void KBlocksPlayManager::clearGamePlayer()
{
    for (int i = 0; i < mPlayerCount; i++) {
        delete maPlayerList[i];
        maPlayerList[i] = nullptr;
    }
    mPlayerCount = 0;
}

void KBlocksPlayManager::startGame()
{
    for (int i = 0; i < mPlayerCount; i++) {
        maPlayerList[i]->startGame(mpGameLogic->getSingleGame(i));
    }
}

void KBlocksPlayManager::stopGame()
{
    for (int i = 0; i < mPlayerCount; i++) {
        maPlayerList[i]->stopGame();
    }
}

void KBlocksPlayManager::pauseGame(bool flag)
{
    for (int i = 0; i < mPlayerCount; i++) {
        maPlayerList[i]->pauseGame(flag);
    }
}
