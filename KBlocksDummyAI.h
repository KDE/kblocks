/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSDUMMYAI_H
#define KBLOCKSDUMMYAI_H

#include "GamePlayerInterface.h"

class KBlocksDummyAI : public GamePlayerInterface
{
public:
    KBlocksDummyAI();
    ~KBlocksDummyAI();

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
