/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include "KBlocksEvent.h"

KBlocksEvent::KBlocksEvent()
{
    mpGameLogic = 0;
}

KBlocksEvent::~KBlocksEvent()
{
}

void KBlocksEvent::setGameLogic(GameLogicInterface * p)
{
    mpGameLogic = p;
}

void KBlocksEvent::startGame()
{
    mpGameLogic->runEvent(GameEvent_Start_Game, -1);
}

void KBlocksEvent::pauseGame()
{
    mpGameLogic->runEvent(GameEvent_Pause_Game, -1);
}

void KBlocksEvent::resumeGame()
{
    mpGameLogic->runEvent(GameEvent_Resume_Game, -1);
}

void KBlocksEvent::stopGame()
{
    mpGameLogic->runEvent(GameEvent_Stop_Game, -1);
}

int KBlocksEvent::runGameAll(int * gameResult)
{
    int gameCount = mpGameLogic->getGameCount();
    
    gameResult = mpGameLogic->runStep(-1);
    
    return gameCount;
}

int KBlocksEvent::runGameByIndex(int gameIndex)
{
    int gameCount = mpGameLogic->getGameCount();
    int* gameResult = new int[gameCount]();
    
    gameResult = mpGameLogic->runStep(gameIndex);
    
    return gameResult[gameIndex];
}
