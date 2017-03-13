/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef PIECE_INTERFACE
#define PIECE_INTERFACE

class PieceInterface
{
public:
    PieceInterface() : mType(0), mPosX(0), mPosY(0), mRotation(0) {};
    virtual ~PieceInterface() {};

public:
    virtual int  toValue() = 0;

    virtual int  getType() = 0;
    virtual int  getPosX() = 0;
    virtual int  getPosY() = 0;
    virtual int  getRotation() = 0;

    virtual int  getCellCount() = 0;
    virtual int  getRotationCount() = 0;
    virtual int  getCellPosX(int index) = 0;
    virtual int  getCellPosY(int index) = 0;

protected:
    int mType;
    int mPosX;
    int mPosY;
    int mRotation;
};

#endif //PIECE_INTERFACE
