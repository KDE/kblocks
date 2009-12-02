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
 * $RCSfile: SingleGameInterface.h,v $
 *
 * $Revision: 1.0 $
 *
 * $Date: 2009/11/01 10:00:00 $
 *
 * AUTHOR: ZhongJie Cai
 *
 * DESCRIPTION: Interface for single game logic of KBlocks 
 *
 ****************************************************************/  
#ifndef SINGLE_GAME_INTERFACE
#define SINGLE_GAME_INTERFACE

#include "FieldInterface.h"
#include "PieceInterface.h"

class GameLogicInterface;

class SingleGameInterface {
    public:
        SingleGameInterface(){};
        virtual ~SingleGameInterface(){};
    
    public:
        virtual FieldInterface* getField() = 0;
        
        virtual int getPieceCount() = 0;
        virtual PieceInterface* getPiece(int index) = 0;
        
        virtual bool setCurrentPiece(int xPos, int yPos, int rotation) = 0;
        
        virtual bool popGameAction(int type, int * param) = 0;
        
    protected:
        FieldInterface* mpField;
        
        int mPieceCount;
        PieceInterface** mpPieceList;
};

#endif //SINGLE_GAME_INTERFACE
