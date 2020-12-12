/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSPIECE_H
#define KBLOCKSPIECE_H

#include "PieceInterface.h"

#define KBlocksPiece_CellCount    4

enum KBlocks_PieceType {
    PieceType_Shape_Z = 0,
    PieceType_Shape_S,
    PieceType_Shape_I,
    PieceType_Shape_T,
    PieceType_Shape_O,
    PieceType_Shape_L,
    PieceType_Shape_J,
    PieceType_Max_Count
};

enum KBlocks_PieceRotation {
    PieceRotation_Up = 0,
    PieceRotation_Left,
    PieceRotation_Down,
    PieceRotation_Right,
    PieceRotation_Max_Count
};

enum KBlocks_PieceType_Detail {
    PieceType_Shape_Z_1 = 0,
    PieceType_Shape_Z_2,
    PieceType_Shape_Z_3,
    PieceType_Shape_Z_4,
    PieceType_Shape_S_1,
    PieceType_Shape_S_2,
    PieceType_Shape_S_3,
    PieceType_Shape_S_4,
    PieceType_Shape_I_1,
    PieceType_Shape_I_2,
    PieceType_Shape_I_3,
    PieceType_Shape_I_4,
    PieceType_Shape_T_1,
    PieceType_Shape_T_2,
    PieceType_Shape_T_3,
    PieceType_Shape_T_4,
    PieceType_Shape_O_1,
    PieceType_Shape_O_2,
    PieceType_Shape_O_3,
    PieceType_Shape_O_4,
    PieceType_Shape_L_1,
    PieceType_Shape_L_2,
    PieceType_Shape_L_3,
    PieceType_Shape_L_4,
    PieceType_Shape_J_1,
    PieceType_Shape_J_2,
    PieceType_Shape_J_3,
    PieceType_Shape_J_4,
    PieceType_Detail_Max_Count
};

class PiecePoint
{
public:
    PiecePoint(int x, int y);
    ~PiecePoint();

public:
    int x();
    int y();

private:
    int mPosX;
    int mPosY;
};

class KBlocksPiece : public PieceInterface
{
public:
    KBlocksPiece();
    explicit KBlocksPiece(PieceInterface *p);
    ~KBlocksPiece();

public:
    void copy(PieceInterface *p);

    int  toValue() override;
    void fromValue(int val);

    int  getType() override;
    void setType(int newType);

    int  getRotation() override;
    void setRotation(int newRotation);

    int  getPosX() override;
    void setPosX(int newPosX);

    int  getPosY() override;
    void setPosY(int newPosY);

    int  getCellCount() override;
    int  getCellPosX(int index) override;
    int  getCellPosY(int index) override;

    int  getWidth();
    int  getHeight();
    int  getRotationCount() override;

    int  getSignature(int *signature);

    void encodeData(unsigned char *data);
    void decodeData(unsigned char *data);
};

#endif

