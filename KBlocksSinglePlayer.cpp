/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksSinglePlayer.h"

KBlocksSinglePlayer::KBlocksSinglePlayer(GamePlayerInterface *player, int thinkInterval, int processInterval)
{
    mpPlayer = player;
    mpGame = 0;

    mPlayerState = KBlocksPlayer_ThinkingState;

    mThinkInterval = thinkInterval;
    mProcessInterval = (processInterval < 0) ? 0 : processInterval;
    if (mThinkInterval >= 0) {
        mActionTimer.setInterval(thinkInterval);
        connect(&mActionTimer, &QTimer::timeout, this, &KBlocksSinglePlayer::doAction);
    }
    mActionTimer.stop();

    mActionList.clear();
}

KBlocksSinglePlayer::~KBlocksSinglePlayer()
{
}

void KBlocksSinglePlayer::startGame(SingleGameInterface *p)
{
    mpGame = p;
    mpPlayer->startGame(mpGame);

    mPlayerState = KBlocksPlayer_ThinkingState;

    mActionList.clear();

    if (mThinkInterval > 0) {
        mActionTimer.start();
    }
}

void KBlocksSinglePlayer::stopGame()
{
    mActionTimer.stop();

    mActionList.clear();

    mpPlayer->stopGame();
    mpGame = 0;
}

void KBlocksSinglePlayer::pauseGame(bool flag)
{
    if (flag) {
        mActionTimer.stop();
    } else if (mThinkInterval > 0) {
        mActionTimer.start();
    }
}

void KBlocksSinglePlayer::think()
{
    mActionList.clear();
    mpPlayer->think(&mActionList);

    if ((!mActionList.empty()) && (mThinkInterval > 0)) {
        mActionTimer.stop();
        mActionTimer.setInterval(mProcessInterval);
        mPlayerState = KBlocksPlayer_ProcessingState;
        mActionTimer.start();
    }
}

bool KBlocksSinglePlayer::process()
{
    if (mActionList.empty()) {
        if (mThinkInterval > 0) {
            mActionTimer.stop();
            mActionTimer.setInterval(mThinkInterval);
            mPlayerState = KBlocksPlayer_ThinkingState;
            mActionTimer.start();
        }
        return false;
    }

    if (!mpGame->isActive()) {
        return false;
    }

    KBlocks_Player_Action tmpAction = mActionList.front();
    mActionList.pop_front();
    switch (tmpAction) {
    case PlayerAction_Move_Left:
        mpGame->setCurrentPiece(-1, 0, 0);
        break;
    case PlayerAction_Move_Right:
        mpGame->setCurrentPiece(1, 0, 0);
        break;
    case PlayerAction_Move_Down:
        mpGame->setCurrentPiece(0, 1, 0);
        break;
    case PlayerAction_Push_Down:
        while (mpGame->setCurrentPiece(0, 1, 0)) ;
        mpGame->forceUpdateGame();
        break;
    case PlayerAction_Rotate_CW:
        mpGame->setCurrentPiece(0, 0, 1);
        break;
    case PlayerAction_Rotate_CCW:
        mpGame->setCurrentPiece(0, 0, -1);
        break;
    case PlayerAction_None:
    default:
        break;
    }

    return true;
}

void KBlocksSinglePlayer::doAction()
{
    if (!mpGame) {
        return;
    }

    if (mPlayerState == KBlocksPlayer_ThinkingState) {
        think();
    } else if (mPlayerState == KBlocksPlayer_ProcessingState) {
        process();
    }
}

