/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksPiece.h"

const int C_PieceRotationCount[PieceType_Max_Count] = {
    2, 2, 2, 4, 1, 4, 4,
};

const PiecePoint C_PieceSize[PieceType_Detail_Max_Count] = {
    //Four blocks, also stair-like (Z)
    PiecePoint(2, 3), PiecePoint(3, 2), PiecePoint(2, 3), PiecePoint(3, 2),

    //Four blocks, stair-like (S)
    PiecePoint(2, 3), PiecePoint(3, 2), PiecePoint(2, 3), PiecePoint(3, 2),

    //Four blocks, straight line (I)
    PiecePoint(1, 4), PiecePoint(4, 1), PiecePoint(1, 4), PiecePoint(4, 1),

    //Four blocks, resembling an airplane (T)
    PiecePoint(2, 3), PiecePoint(3, 2), PiecePoint(2, 3), PiecePoint(3, 2),

    //Four blocks, square (O)
    PiecePoint(2, 2), PiecePoint(2, 2), PiecePoint(2, 2), PiecePoint(2, 2),

    //Four blocks, L shape (L)
    PiecePoint(2, 3), PiecePoint(3, 2), PiecePoint(2, 3), PiecePoint(3, 2),

    //Four blocks, also (J)
    PiecePoint(2, 3), PiecePoint(3, 2), PiecePoint(2, 3), PiecePoint(3, 2),
};

const PiecePoint C_PieceOffset[PieceType_Detail_Max_Count] = {
    //Four blocks, also stair-like (Z)
    PiecePoint(-1, -1), PiecePoint(-1, 0), PiecePoint(-1, -1), PiecePoint(-1, 0),

    //Four blocks, stair-like (S)
    PiecePoint(-1, -1), PiecePoint(-1, 0), PiecePoint(-1, -1), PiecePoint(-1, 0),

    //Four blocks, straight line (I)
    PiecePoint(0, -1), PiecePoint(-1, 0), PiecePoint(0, -1), PiecePoint(-1, 0),

    //Four blocks, resembling an airplane (T)
    PiecePoint(0, -1), PiecePoint(-1, -1), PiecePoint(-1, -1), PiecePoint(-1, 0),

    //Four blocks, square (O)
    PiecePoint(-1, -1), PiecePoint(-1, -1), PiecePoint(-1, -1), PiecePoint(-1, -1),

    //Four blocks, L shape (L)
    PiecePoint(0, -1), PiecePoint(-1, -1), PiecePoint(-1, -1), PiecePoint(-1, 0),

    //Four blocks, also (J)
    PiecePoint(-1, -1), PiecePoint(-1, 0), PiecePoint(0, -1), PiecePoint(-1, -1),
};

const PiecePoint C_PieceCell[PieceType_Detail_Max_Count][KBlocksPiece_CellCount] = {
    //Four blocks, also stair-like (Z)
    { PiecePoint(0, 1), PiecePoint(1, 0), PiecePoint(1, 1), PiecePoint(0, 2) },
    { PiecePoint(0, 0), PiecePoint(1, 0), PiecePoint(1, 1), PiecePoint(2, 1) },
    { PiecePoint(0, 1), PiecePoint(1, 0), PiecePoint(1, 1), PiecePoint(0, 2) },
    { PiecePoint(0, 0), PiecePoint(1, 0), PiecePoint(1, 1), PiecePoint(2, 1) },

    //Four blocks, stair-like (S)
    { PiecePoint(0, 0), PiecePoint(0, 1), PiecePoint(1, 1), PiecePoint(1, 2) },
    { PiecePoint(0, 1), PiecePoint(1, 1), PiecePoint(1, 0), PiecePoint(2, 0) },
    { PiecePoint(0, 0), PiecePoint(0, 1), PiecePoint(1, 1), PiecePoint(1, 2) },
    { PiecePoint(0, 1), PiecePoint(1, 1), PiecePoint(1, 0), PiecePoint(2, 0) },

    //Four blocks, straight line (I)
    { PiecePoint(0, 0), PiecePoint(0, 1), PiecePoint(0, 2), PiecePoint(0, 3) },
    { PiecePoint(0, 0), PiecePoint(1, 0), PiecePoint(2, 0), PiecePoint(3, 0) },
    { PiecePoint(0, 0), PiecePoint(0, 1), PiecePoint(0, 2), PiecePoint(0, 3) },
    { PiecePoint(0, 0), PiecePoint(1, 0), PiecePoint(2, 0), PiecePoint(3, 0) },

    //Four blocks, resembling an airplane (T)
    { PiecePoint(0, 0), PiecePoint(0, 1), PiecePoint(0, 2), PiecePoint(1, 1) },
    { PiecePoint(0, 1), PiecePoint(1, 1), PiecePoint(2, 1), PiecePoint(1, 0) },
    { PiecePoint(1, 2), PiecePoint(0, 1), PiecePoint(1, 1), PiecePoint(1, 0) },
    { PiecePoint(0, 0), PiecePoint(1, 0), PiecePoint(1, 1), PiecePoint(2, 0) },

    //Four blocks, square (O)
    { PiecePoint(0, 0), PiecePoint(1, 0), PiecePoint(0, 1), PiecePoint(1, 1) },
    { PiecePoint(0, 0), PiecePoint(1, 0), PiecePoint(0, 1), PiecePoint(1, 1) },
    { PiecePoint(0, 0), PiecePoint(1, 0), PiecePoint(0, 1), PiecePoint(1, 1) },
    { PiecePoint(0, 0), PiecePoint(1, 0), PiecePoint(0, 1), PiecePoint(1, 1) },

    //Four blocks, L shape (L)
    { PiecePoint(0, 2), PiecePoint(1, 2), PiecePoint(1, 1), PiecePoint(1, 0) },
    { PiecePoint(0, 0), PiecePoint(1, 0), PiecePoint(2, 0), PiecePoint(2, 1) },
    { PiecePoint(0, 0), PiecePoint(0, 1), PiecePoint(0, 2), PiecePoint(1, 0) },
    { PiecePoint(0, 0), PiecePoint(0, 1), PiecePoint(1, 1), PiecePoint(2, 1) },

    //Four blocks, also (J)
    { PiecePoint(0, 0), PiecePoint(0, 1), PiecePoint(0, 2), PiecePoint(1, 2) },
    { PiecePoint(0, 1), PiecePoint(1, 1), PiecePoint(2, 1), PiecePoint(2, 0) },
    { PiecePoint(0, 0), PiecePoint(1, 0), PiecePoint(1, 1), PiecePoint(1, 2) },
    { PiecePoint(0, 0), PiecePoint(1, 0), PiecePoint(2, 0), PiecePoint(0, 1) },
};

//-----------------------------------------------------------------------------
PiecePoint::PiecePoint(int x, int y)
{
    mPosX = x;
    mPosY = y;
}

PiecePoint::~PiecePoint()
{
}

int PiecePoint::x()
{
    return mPosX;
}

int PiecePoint::y()
{
    return mPosY;
}
//-----------------------------------------------------------------------------

KBlocksPiece::KBlocksPiece()
{
}

KBlocksPiece::KBlocksPiece(PieceInterface *p)
{
    copy(p);
}

KBlocksPiece::~KBlocksPiece()
{
}

void KBlocksPiece::copy(PieceInterface *p)
{
    mType = p->getType();
    mPosX = p->getPosX();
    mPosY = p->getPosY();
    mRotation = p->getRotation();
}

int KBlocksPiece::toValue()
{
    return (mType * PieceRotation_Max_Count + mRotation);
}

void KBlocksPiece::fromValue(int val)
{
    mType = val / PieceRotation_Max_Count;
    mRotation = val % PieceRotation_Max_Count;
}

int KBlocksPiece::getType()
{
    return mType;
}

void KBlocksPiece::setType(int newType)
{
    mType = (newType + PieceType_Max_Count) % PieceType_Max_Count;
}

int KBlocksPiece::getRotation()
{
    return mRotation;
}

void KBlocksPiece::setRotation(int newRotation)
{
    mRotation = (newRotation + PieceRotation_Max_Count) % PieceRotation_Max_Count;
}

int KBlocksPiece::getPosX()
{
    return mPosX;
}

void KBlocksPiece::setPosX(int newPosX)
{
    mPosX = newPosX;
}

int KBlocksPiece::getPosY()
{
    return mPosY;
}

void KBlocksPiece::setPosY(int newPosY)
{
    mPosY = newPosY;
}

int KBlocksPiece::getCellCount()
{
    return KBlocksPiece_CellCount;
}

int KBlocksPiece::getCellPosX(int index)
{
    if ((index < 0) || (index >= KBlocksPiece_CellCount)) {
        return -1;
    }

    PiecePoint tmpCell = C_PieceCell[mType * PieceRotation_Max_Count + mRotation][index];
    PiecePoint tmpOffset = C_PieceOffset[mType * PieceRotation_Max_Count + mRotation];

    return (mPosX + tmpCell.x() + tmpOffset.x());
}

int KBlocksPiece::getCellPosY(int index)
{
    if ((index < 0) || (index >= KBlocksPiece_CellCount)) {
        return -1;
    }

    PiecePoint tmpCell = C_PieceCell[mType * PieceRotation_Max_Count + mRotation][index];
    PiecePoint tmpOffset = C_PieceOffset[mType * PieceRotation_Max_Count + mRotation];

    return (mPosY + tmpCell.y() + tmpOffset.y());
}

int KBlocksPiece::getWidth()
{
    PiecePoint tmpPoint = C_PieceSize[mType * PieceRotation_Max_Count + mRotation];
    return tmpPoint.x();
}

int KBlocksPiece::getHeight()
{
    PiecePoint tmpPoint = C_PieceSize[mType * PieceRotation_Max_Count + mRotation];
    return tmpPoint.y();
}

int KBlocksPiece::getRotationCount()
{
    return C_PieceRotationCount[mType];
}

int KBlocksPiece::getSignature(int *signature)
{
    for (int i = 0; i < KBlocksPiece_CellCount; i++) {
        signature[i] = -1;
    }

    for (int i = 0; i < KBlocksPiece_CellCount; i++) {
        PiecePoint tmpPoint = C_PieceCell[mType * PieceRotation_Max_Count + mRotation][i];
        if (tmpPoint.y() > signature[tmpPoint.x()]) {
            signature[tmpPoint.x()] = tmpPoint.y();
        }
    }

    return getWidth();
}

void KBlocksPiece::encodeData(unsigned char *data)
{
    data[0] = mType;
    data[1] = mPosX;
    data[2] = mPosY;
    data[3] = mRotation;
}

void KBlocksPiece::decodeData(unsigned char *data)
{
    mType = data[0];
    mPosX = data[1];
    mPosY = data[2];
    mRotation = data[3];
}
