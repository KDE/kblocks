/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksGameLogic.h"

KBlocksGameLogic::KBlocksGameLogic(int capacity)
{
    mGameCount = 0;
    mGameMax = capacity;
    
    mGameSeed = 0;
    mPunishFlag = true;
    
    mStandbyMode = false;
    mGameInterval = 0;
    
    mRecordMode = false;
    mReplayMode = false;
    
    maGameList = new KBlocksSingleGame*[capacity]();
}

KBlocksGameLogic::KBlocksGameLogic(char* replay)
{
    mReplayMode = true;
}

KBlocksGameLogic::~KBlocksGameLogic()
{
    delete [] maGameList;
}

int KBlocksGameLogic::getActiveGameCount()
{
    int result = 0;
    for(int i = 0; i < mGameCount; i++)
    {
        if (maGameList[i]->isGameRunning())
        {
            result++;
        }
    }
    return result;
}

KBlocksSingleGame* KBlocksGameLogic::getSingleGame(int index)
{
    if ((index < 0) || (index >= mGameCount))
    {
        return 0;
    }
    return maGameList[index];
}

int KBlocksGameLogic::levelUpGame(int level)
{
    mGameInterval -= (mLevelUpInterval * level);
    if (mGameInterval < (mLevelUpInterval * 2))
    {
        mGameInterval = mLevelUpInterval;
    }
    setGameInterval(mGameInterval);
    return mGameInterval;
}

int KBlocksGameLogic::updateGame(int * lineList)
{
    for(int i = 0; i < mGameCount; i++)
    {
        int tmpTotal = 0;
        int tmpValue = 0;
        int tmpPunishCount = 0;
        
        while(maGameList[i]->pickGameResult(&tmpValue))
        {
            if (tmpValue < 0)
            {
                tmpTotal = -1;
                tmpPunishCount = 0;
                break;
            }
            tmpTotal += tmpValue;
            tmpPunishCount += (tmpValue - 1);
        }
        
        lineList[i] = tmpTotal;
        
        if ((mPunishFlag) && (tmpPunishCount > 0))
        {
            int punishSeed = rand();
            for(int j = 0; j < i; j++)
            {
                maGameList[j]->punishGame(tmpPunishCount, punishSeed);
            }
            for(int j = i + 1; j < mGameCount; j++)
            {
                maGameList[j]->punishGame(tmpPunishCount, punishSeed);
            }
        }
    }
    
    return mGameCount;
}

void KBlocksGameLogic::setGameSeed(int seed)
{
    mGameSeed = seed;
}

void KBlocksGameLogic::setGamePunish(bool flag)
{
    mPunishFlag = flag;
}

void KBlocksGameLogic::setGameStandbyMode(bool flag)
{
    mStandbyMode = flag;
    for(int i = 0; i < mGameCount; i++)
    {
        maGameList[i]->setGameStandbyMode(mStandbyMode);
    }
}

void KBlocksGameLogic::setGameInterval(int interval)
{
    mGameInterval = interval;
    for(int i = 0; i < mGameCount; i++)
    {
        maGameList[i]->setGameInterval(mGameInterval);
    }
}

void KBlocksGameLogic::setInitInterval(int interval)
{
    mInitialInterval = interval;
}

void KBlocksGameLogic::setLevelUpInterval(int interval)
{
    mLevelUpInterval = interval;
}

bool KBlocksGameLogic::startGame(int gameCount)
{
    if (mGameCount != 0)
    {
        return false;
    }
    mGameInterval = mInitialInterval;
    createSingleGames(gameCount);
    return true;
}

bool KBlocksGameLogic::stopGame()
{
    if (mGameCount == 0)
    {
        return false;
    }
    deleteSingleGames();
    return true;
}

void KBlocksGameLogic::pauseGame(bool pauseFlag)
{
    for(int i = 0; i < mGameCount; i++)
    {
        maGameList[i]->pauseGame(pauseFlag);
    }
}

void KBlocksGameLogic::continueGame()
{
    for(int i = 0; i < mGameCount; i++)
    {
        maGameList[i]->continueGame();
    }
}

void KBlocksGameLogic::createSingleGames(int gameCount)
{
    if (gameCount > mGameMax)
    {
        gameCount = mGameMax;
    }
    mGameCount = gameCount;
    
    int seedList[mGameCount];
    if (mGameSeed < 0)
    {
        mGameSeed = -mGameSeed;
        srand(mGameSeed);
        for(int i = 0; i < mGameCount; i++)
        {
            seedList[i] = rand();
        }
    }
    else
    {
        for(int i = 0; i < mGameCount; i++)
        {
            seedList[i] = mGameSeed;
        }
    }
    
    for(int i = 0; i < mGameCount; i++)
    {
        maGameList[i] = new KBlocksSingleGame(i);
        maGameList[i]->setGameStandbyMode(mStandbyMode);
        maGameList[i]->setGameInterval(mGameInterval);
        maGameList[i]->startGame(seedList[i]);
    }
}

void KBlocksGameLogic::deleteSingleGames()
{
    for(int i = 0; i < mGameCount; i++)
    {
        maGameList[i]->stopGame();
        delete maGameList[i];
        maGameList[i] = 0;
    }
    mGameCount = 0;
}
