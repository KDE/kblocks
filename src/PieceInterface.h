/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
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
