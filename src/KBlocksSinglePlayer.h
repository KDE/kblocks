/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
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
    ~KBlocksSinglePlayer() override;

public:
    void startGame(SingleGameInterface *p);
    void stopGame();

    void pauseGame(bool flag);

private:
    void think();
    bool process();

private Q_SLOTS:
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

