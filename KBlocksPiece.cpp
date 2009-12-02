/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include <QPoint>

#include "KBlocksPiece.h"

const QPoint C_PieceOffset[PieceType_Detail_Max_Count][4] = {
    //Four blocks, straight line (I)
    { QPoint(-1,0), QPoint(0,0), QPoint(1,0), QPoint(2,0) },
    { QPoint(0,-1), QPoint(0,0), QPoint(0,1), QPoint(0,2) },
    { QPoint(-1,0), QPoint(0,0), QPoint(1,0), QPoint(2,0) },
    { QPoint(0,-1), QPoint(0,0), QPoint(0,1), QPoint(0,2) },

    //Four blocks, L shape (L)
    { QPoint(0,0), QPoint(0,-1), QPoint(0,-2), QPoint(1,0) },
    { QPoint(0,0), QPoint(-1,0), QPoint(-2,0), QPoint(0,-1) },
    { QPoint(0,0), QPoint(0,1), QPoint(0,2), QPoint(-1,0) },
    { QPoint(0,0), QPoint(1,0), QPoint(2,0), QPoint(0,1) },

    //Four blocks, also (J)
    { QPoint(0,0), QPoint(0,-1), QPoint(0,-2), QPoint(-1,0) },
    { QPoint(0,0), QPoint(-1,0), QPoint(-2,0), QPoint(0,1) },
    { QPoint(0,0), QPoint(0,1), QPoint(0,2), QPoint(1,0) },
    { QPoint(0,0), QPoint(1,0), QPoint(2,0), QPoint(0,-1) },

    //Four blocks, also stair-like (Z)
    { QPoint(0,0), QPoint(0,-1), QPoint(1,0), QPoint(1,1) },
    { QPoint(0,0), QPoint(-1,0), QPoint(0,-1), QPoint(1,-1) },
    { QPoint(0,0), QPoint(0,-1), QPoint(1,0), QPoint(1,1) },
    { QPoint(0,0), QPoint(-1,0), QPoint(0,-1), QPoint(1,-1) },

    //Four blocks, stair-like (S)
    { QPoint(0,0), QPoint(0,1), QPoint(1,0), QPoint(1,-1) },
    { QPoint(0,0), QPoint(-1,0), QPoint(0,1), QPoint(1,1) },
    { QPoint(0,0), QPoint(0,1), QPoint(1,0), QPoint(1,-1) },
    { QPoint(0,0), QPoint(-1,0), QPoint(0,1), QPoint(1,1) },

    //Four blocks, resembling an airplane (T)
    { QPoint(0,0), QPoint(0,-1), QPoint(1,0), QPoint(-1,0) },
    { QPoint(0,0), QPoint(0,-1), QPoint(-1,0), QPoint(0,1) },
    { QPoint(0,0), QPoint(1,0), QPoint(-1,0), QPoint(0,1) }, 
    { QPoint(0,0), QPoint(0,-1), QPoint(1,0), QPoint(0,1) },

    //Four blocks, square (O)
    { QPoint(0,0), QPoint(1,0), QPoint(0,-1), QPoint(1,-1) },
    { QPoint(0,0), QPoint(1,0), QPoint(0,-1), QPoint(1,-1) },
    { QPoint(0,0), QPoint(1,0), QPoint(0,-1), QPoint(1,-1) },
    { QPoint(0,0), QPoint(1,0), QPoint(0,-1), QPoint(1,-1) },
};

KBlocksPiece::KBlocksPiece()
{
    type = 0;
    posX = 0;
    posY = 0;
    rotation = 0;
}

KBlocksPiece::~KBlocksPiece()
{
}

void KBlocksPiece::fromValue(int val)
{
    type = val / PieceRotation_Max_Count;
    rotation = val % PieceRotation_Max_Count;
}

int KBlocksPiece::toValue()
{
    return (type * PieceRotation_Max_Count + rotation);
}

int KBlocksPiece::getType()
{
    return type;
}

void KBlocksPiece::setType(int newType)
{
    type = (newType + PieceType_Max_Count) % PieceType_Max_Count;
}

int KBlocksPiece::getRotation()
{
    return rotation;
}

void KBlocksPiece::setRotation(int newRotation)
{
    rotation = (newRotation + PieceRotation_Max_Count) % PieceRotation_Max_Count;
}

int KBlocksPiece::getPosX()
{
    return posX;
}

void KBlocksPiece::setPosX(int newPosX)
{
    posX = newPosX;
}

int KBlocksPiece::getPosY()
{
    return posY;
}

void KBlocksPiece::setPosY(int newPosY)
{
    posY = newPosY;
}

int KBlocksPiece::getCellPosX(int index)
{
    if ((index < 0) || (index > 4))
    {
        return -1;
    }
    
    QPoint* tmpCoord = new QPoint(posX, posY);
    QPoint tmpPoint = C_PieceOffset[type * PieceRotation_Max_Count + rotation][index];
    
    *tmpCoord += tmpPoint;
    
    return tmpCoord->x();
}

int KBlocksPiece::getCellPosY(int index)
{
    if ((index < 0) || (index > 4))
    {
        return -1;
    }
    
    QPoint* tmpCoord = new QPoint(posX, posY);
    QPoint tmpPoint = C_PieceOffset[type * PieceRotation_Max_Count + rotation][index];
    
    *tmpCoord += tmpPoint;
    
    return tmpCoord->y();
}
