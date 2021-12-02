/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
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

