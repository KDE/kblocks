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

#include "GameLogicInterface.h"
#include "SingleGameInterface.h"

#include "KBlocksSingleGame.h"

#include "KBlocksDefine.h"

class KBlocksGameLogic : public GameLogicInterface {
    public:
        KBlocksGameLogic(int capacity);
        ~KBlocksGameLogic();
        
    public:
        void createSingleGames(int gameCount);
        void deleteSingleGames();
        
        void setGameSeed(int seed);
        
        int getGameMaxCapacity();
        int getGameCount();
        KBlocksSingleGame* getSingleGame(int index);
        
        int* runStep(int gameIndex = -1);
        int runEvent(int gameEvent, int gameIndex = -1);
        
    protected:
        KBlocksSingleGame** maGameList;
        
    private:
        int mGameMax;
        int mGameCount;
		int mGameSeed;
};

#endif
