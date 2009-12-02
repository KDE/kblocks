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
 * $RCSfile: PieceInterface.h,v $
 *
 * $Revision: 1.0 $
 *
 * $Date: 2009/11/01 10:00:00 $
 *
 * AUTHOR: ZhongJie Cai
 *
 * DESCRIPTION: Interface for falling pieces of KBlocks 
 *
 ****************************************************************/  
#ifndef PIECE_INTERFACE
#define PIECE_INTERFACE

class PieceInterface {
    protected:
        int type;
        int posX;
        int posY;
        int rotation;
    
    public:
        PieceInterface(){};
        virtual ~PieceInterface(){};
    
    public:
        virtual void fromValue(int val) = 0;
        virtual int  toValue() = 0;
        
        virtual int  getType() = 0;
        virtual void setType(int newType) = 0;
        
        virtual int  getPosX() = 0;
        virtual void setPosX(int newPosX) = 0;
        
        virtual int  getPosY() = 0;
        virtual void setPosY(int newPosY) = 0;
        
        virtual int  getRotation() = 0;
        virtual void setRotation(int newRotation) = 0;
        
        virtual int  getCellPosX(int index) = 0;
        virtual int  getCellPosY(int index) = 0;
};

#endif //PIECE_INTERFACE
