/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSGAMELOGIC_H
#define KBLOCKSGAMELOGIC_H

#include <stdlib.h>

#include "GameLogicInterface.h"
#include "SingleGameInterface.h"

#include "KBlocksSingleGame.h"

#include "KBlocksDefine.h"

class KBlocksGameLogic : public GameLogicInterface
{
    public:
        KBlocksGameLogic(int capacity);
        KBlocksGameLogic(char* replay);
        ~KBlocksGameLogic();
        
    public:
        int getActiveGameCount();
        KBlocksSingleGame* getSingleGame(int index);
        
        void setRecordMode(bool flag);
        
        int levelUpGame(int level);
        int updateGame(int * lineList);
        
        void setGameSeed(int seed);
        void setGamePunish(bool flag);
        
        void setGameStandbyMode(bool flag);
        void setGameInterval(int interval);
        void setInitInterval(int interval);
        void setLevelUpInterval(int interval);
        
        bool startGame(int gameCount);
        bool stopGame();
        
        void pauseGame(bool pauseFlag);
        void continueGame();
        
    private:
        void createSingleGames(int gameCount);
        void deleteSingleGames();
        
    protected:
        KBlocksSingleGame** maGameList;
        
    private:
        int mGameMax;
        int mGameCount;
        
		int mGameSeed;
		int mPunishFlag;
		
        bool mStandbyMode;
        int mGameInterval;
        int mInitialInterval;
        int mLevelUpInterval;
		
		bool mRecordMode;
		bool mReplayMode;
};

#endif
