/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef KBLOCKSCONTROL_H
#define KBLOCKSCONTROL_H

#include "SingleGameInterface.h"

#include "KBlocksDefine.h"

class KBlocksControl {
    public:
        KBlocksControl();
        ~KBlocksControl();
        
        void setSingleGame(SingleGameInterface * p);
        
    public:
        bool rotateCW();
        bool rotateCCW();
        bool moveLeft();
        bool moveRight();
        bool moveDown();
        bool pushDown();
        
        int getRemovedLines(int * lineList);
        int getRemovedCount();
        
        int getNewPiecePosX(int * posX);
        int getNewPiecePosY(int * posY);
        
    private:
        SingleGameInterface* mpSingleGame;
};

#endif
