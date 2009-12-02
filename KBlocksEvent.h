/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef KBLOCKSEVENT_H
#define KBLOCKSEVENT_H

#include "GameLogicInterface.h"

#include "KBlocksDefine.h"

class KBlocksEvent
{
    public:
        KBlocksEvent();
        ~KBlocksEvent();
        
        void setGameLogic(GameLogicInterface * p);
        
        void startGame();
        void pauseGame();
        void resumeGame();
        void stopGame();
        
        int runGameAll(int * gameResult);
        int runGameByIndex(int gameIndex);

    protected:
        GameLogicInterface* mpGameLogic;
};

#endif
