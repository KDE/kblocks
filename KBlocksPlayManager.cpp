/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
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
        maPlayerList[i] = 0;
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
