/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

/****************************************************************
 *
 * $RCSfile: GameLogicInterface.h,v $
 *
 * $Revision: 1.0 $
 *
 * $Date: 2009/11/01 10:00:00 $
 *
 * AUTHOR: ZhongJie Cai
 *
 * DESCRIPTION: Interface for multi game logic of KBlocks 
 *
 ****************************************************************/  
#ifndef GAME_LOGIC_INTERFACE
#define GAME_LOGIC_INTERFACE

#include "SingleGameInterface.h"

class GameLogicInterface {
    public:
        GameLogicInterface(){};
        virtual ~GameLogicInterface(){};
    
    public:
        virtual void createSingleGames(int gameCount) = 0;
        virtual void deleteSingleGames() = 0;
        
        virtual void setGameSeed(int seed) = 0;
        
        virtual int getGameMaxCapacity() = 0;
        virtual int getGameCount() = 0;
        virtual SingleGameInterface* getSingleGame(int index) = 0;
        
        virtual int* runStep(int gameIndex) = 0;
        virtual int runEvent(int gameEvent, int gameIndex) = 0;
        
    protected:
        SingleGameInterface** maGameList;
};

#endif //GAME_LOGIC_INTERFACE
