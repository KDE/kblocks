/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSPLAYMANAGER_H
#define KBLOCKSPLAYMANAGER_H

#include "GamePlayerInterface.h"
#include "GameLogicInterface.h"
#include "KBlocksSinglePlayer.h"

class KBlocksPlayManager
{
public:
    KBlocksPlayManager(GameLogicInterface *p, int capacity);
    ~KBlocksPlayManager();

public:
    bool addGamePlayer(GamePlayerInterface *p, int thinkInterval, int processInterval);
    void clearGamePlayer();

    void startGame();
    void stopGame();

    void pauseGame(bool flag);

private:
    GameLogicInterface *mpGameLogic = nullptr;

    int mPlayerCount;
    int mMaxCapacity;
    KBlocksSinglePlayer **maPlayerList;
};

#endif

