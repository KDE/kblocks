/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef GAME_LOGIC_INTERFACE
#define GAME_LOGIC_INTERFACE

#include "SingleGameInterface.h"

class GameLogicInterface
{
    public:
        GameLogicInterface(){};
        virtual ~GameLogicInterface(){};
    
    public:
        virtual SingleGameInterface* getSingleGame(int) = 0;
        
        virtual int levelUpGame(int) = 0;
        virtual int updateGame(int*) = 0;
        
        virtual bool startGame(int) = 0;
        virtual bool stopGame() = 0;
        
        virtual void pauseGame(bool) = 0;
        virtual void continueGame() = 0;
        
    protected:
        SingleGameInterface** maGameList;
};

#endif //GAME_LOGIC_INTERFACE
