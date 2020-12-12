/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSSINGLEPLAYER_H
#define KBLOCKSSINGLEPLAYER_H

#include <QTimer>

#include "SingleGameInterface.h"
#include "GamePlayerInterface.h"

enum KBlocksPlayer_State {
    KBlocksPlayer_ThinkingState = 0,
    KBlocksPlayer_ProcessingState,
    KBlocksPlayer_Max_State_Count
};

class KBlocksSinglePlayer : public QObject
{
    Q_OBJECT

public:
    KBlocksSinglePlayer(GamePlayerInterface *player, int thinkInterval, int processInterval);
    ~KBlocksSinglePlayer();

public:
    void startGame(SingleGameInterface *p);
    void stopGame();

    void pauseGame(bool flag);

private:
    void think();
    bool process();

private slots:
    void doAction();

private:
    GamePlayerInterface *mpPlayer = nullptr;
    SingleGameInterface *mpGame = nullptr;

    int mPlayerState;

    int mThinkInterval;
    int mProcessInterval;

    QTimer mActionTimer;

    GamePlayer_ActionList mActionList;
};

#endif

