/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#ifndef KBLOCKSDUMMYAI_H
#define KBLOCKSDUMMYAI_H

#include "GamePlayerInterface.h"

class KBlocksDummyAI : public GamePlayerInterface
{
public:
    KBlocksDummyAI();
    ~KBlocksDummyAI() override;

public:
    void startGame(SingleGameInterface *p) override;
    void stopGame() override;

    void pauseGame(bool flag) override;

    void think(GamePlayer_ActionList *actionList) override;

protected:
    bool mPauseFlag;

    int mFieldWidth;
    int mRotateCount;
};

#endif
