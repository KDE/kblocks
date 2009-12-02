/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef KBLOCKSPIECE_H
#define KBLOCKSPIECE_H

#include "PieceInterface.h"

enum KBlocks_PieceType {
    PieceType_Shape_I = 0,
    PieceType_Shape_L,
    PieceType_Shape_J,
    PieceType_Shape_Z,
    PieceType_Shape_S,
    PieceType_Shape_T,
    PieceType_Shape_O,
    PieceType_Max_Count,
};

enum KBlocks_PieceRotation {
    PieceRotation_Up = 0,
    PieceRotation_Left,
    PieceRotation_Down,
    PieceRotation_Right,
    PieceRotation_Max_Count,
};

enum KBlocks_PieceType_Detail {
    PieceType_Shape_I_1 = 0,
    PieceType_Shape_I_2,
    PieceType_Shape_I_3,
    PieceType_Shape_I_4,
    PieceType_Shape_L_1,
    PieceType_Shape_L_2,
    PieceType_Shape_L_3,
    PieceType_Shape_L_4,
    PieceType_Shape_J_1,
    PieceType_Shape_J_2,
    PieceType_Shape_J_3,
    PieceType_Shape_J_4,
    PieceType_Shape_Z_1,
    PieceType_Shape_Z_2,
    PieceType_Shape_Z_3,
    PieceType_Shape_Z_4,
    PieceType_Shape_S_1,
    PieceType_Shape_S_2,
    PieceType_Shape_S_3,
    PieceType_Shape_S_4,
    PieceType_Shape_T_1,
    PieceType_Shape_T_2,
    PieceType_Shape_T_3,
    PieceType_Shape_T_4,
    PieceType_Shape_O_1,
    PieceType_Shape_O_2,
    PieceType_Shape_O_3,
    PieceType_Shape_O_4,
    PieceType_Detail_Max_Count,
};

class KBlocksPiece : public PieceInterface {
    protected:
        int type;
        int rotation;
        
        int posX;
        int posY;
    
    public:
        KBlocksPiece();
        ~KBlocksPiece();
    
    public:
        void fromValue(int val);
        int  toValue();
        
        int  getType();
        void setType(int newType);
        
        int  getRotation();
        void setRotation(int newRotation);
        
        int  getPosX();
        void setPosX(int newPosX);
        
        int  getPosY();
        void setPosY(int newPosY);
        
        int  getCellPosX(int index);
        int  getCellPosY(int index);
};

#endif
