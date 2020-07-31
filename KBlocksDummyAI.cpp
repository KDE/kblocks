/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksDummyAI.h"

#include <QtGlobal>
#include <QRandomGenerator>

KBlocksDummyAI::KBlocksDummyAI()
{
    mPauseFlag = false;

    mFieldWidth = 0;
    mRotateCount = 0;
}

KBlocksDummyAI::~KBlocksDummyAI()
{
}

void KBlocksDummyAI::startGame(SingleGameInterface *p)
{
    mpGame = p;
    mPauseFlag = false;

    mFieldWidth = mpGame->getField()->getWidth();
    mRotateCount = mpGame->getPiece(0)->getRotationCount();
}

void KBlocksDummyAI::stopGame()
{
    mpGame = 0;
}

void KBlocksDummyAI::pauseGame(bool flag)
{
    mPauseFlag = flag;
}

void KBlocksDummyAI::think(GamePlayer_ActionList *actionList)
{
    if (mPauseFlag) {
        return;
    }

    auto random = QRandomGenerator::global();
    int rotation = random->bounded(mRotateCount + 1) - mRotateCount / 2;
    int motionx = random->bounded(mFieldWidth + 1) - mFieldWidth / 2;

    if (rotation > 0) {
        for (int i = 0; i < rotation; i++) {
            actionList->push_back(PlayerAction_Rotate_CW);
        }
    } else {
        rotation = -rotation;
        for (int i = 0; i < rotation; i++) {
            actionList->push_back(PlayerAction_Rotate_CCW);
        }
    }

    if (motionx > 0) {
        for (int i = 0; i < motionx; i++) {
            actionList->push_back(PlayerAction_Move_Right);
        }
    } else {
        motionx = -motionx;
        for (int i = 0; i < motionx; i++) {
            actionList->push_back(PlayerAction_Move_Left);
        }
    }

    //actionList->push_back(PlayerAction_Push_Down);
}
