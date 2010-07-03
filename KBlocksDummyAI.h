/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSDUMMYAI_H
#define KBLOCKSDUMMYAI_H

#include "GamePlayerInterface.h"

class KBlocksDummyAI : public GamePlayerInterface
{
    public:
        KBlocksDummyAI();
        ~KBlocksDummyAI();
        
    public:
        void startGame(SingleGameInterface * p);
        void stopGame();
        
        void pauseGame(bool flag);
        
        void think(GamePlayer_ActionList * actionList);
        
    protected:
        SingleGameInterface* mpGame;
        bool mPauseFlag;
        
        int mFieldWidth;
        int mRotateCount;
};

#endif
