/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include <stdlib.h>

#include "KBlocksGameLogic.h"

KBlocksGameLogic::KBlocksGameLogic(int capacity)
{
    mGameSeed = 0;
    mGameMax = capacity;
}

KBlocksGameLogic::~KBlocksGameLogic()
{
}

void KBlocksGameLogic::createSingleGames(int gameCount)
{
    if (gameCount > mGameMax)
    {
        gameCount = mGameMax;
    }
    mGameCount = gameCount;
    
    maGameList = new KBlocksSingleGame*[mGameCount]();
    for(int i = 0; i < mGameCount; i++)
    {
        maGameList[i] = new KBlocksSingleGame();
        maGameList[i]->startGame(mGameSeed);
    }
}

void KBlocksGameLogic::deleteSingleGames()
{
    for(int i = 0; i < mGameCount; i++)
    {
        delete maGameList[i];
    }
    delete [] maGameList;
}

void KBlocksGameLogic::setGameSeed(int seed)
{
    mGameSeed = seed;
}

int KBlocksGameLogic::getGameMaxCapacity()
{
    return mGameMax;
}

int KBlocksGameLogic::getGameCount()
{
    return mGameCount;
}

KBlocksSingleGame* KBlocksGameLogic::getSingleGame(int index)
{
    if ((index < 0) || (index > mGameCount))
    {
        return 0;
    }
    return maGameList[index];
}

int* KBlocksGameLogic::runStep(int gameIndex)
{
    int* result = new int[mGameCount]();
    
    for(int i = 0; i < mGameCount; i++)
    {
        result[i] = 0;
    }
    
    if (gameIndex == -1)
    {
        for(int i = 0; i < mGameCount; i++)
        {
            result[i] = maGameList[i]->runStep();
        }
    }
    else
    {
        result[gameIndex] = maGameList[gameIndex]->runStep();
    }
    
    return result;
}

int KBlocksGameLogic::runEvent(int gameEvent, int gameIndex)
{
    int result = 0;
    
    if (gameEvent == GameEvent_Start_Game)
    {
        for(int i = 0; i < mGameCount; i++)
        {
            maGameList[i]->startGame(mGameSeed);
        }
    }
    else if (gameEvent == GameEvent_Pause_Game)
    {
        for(int i = 0; i < mGameCount; i++)
        {
            maGameList[i]->pauseGame();
        }
    }
    else if (gameEvent == GameEvent_Resume_Game)
    {
        for(int i = 0; i < mGameCount; i++)
        {
            maGameList[i]->resumeGame();
        }
    }
    else if (gameEvent == GameEvent_Stop_Game)
    {
        for(int i = 0; i < mGameCount; i++)
        {
            maGameList[i]->stopGame();
        }
    }
    else
    {
        if (gameIndex == -1)
        {
            for(int i = 0; i < mGameCount; i++)
            {
                maGameList[i]->runEvent(gameEvent);
            }
        }
        else
        {
            for(int i = 0; i < gameIndex; i++)
            {
                maGameList[i]->runEvent(gameEvent);
            }
            for(int i = gameIndex + 1; i < mGameCount; i++)
            {
                maGameList[i]->runEvent(gameEvent);
            }
        }
    }
    
    return result;
}
