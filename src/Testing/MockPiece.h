/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/
#ifndef MOCKPIECE_H
#define MOCKPIECE_H

#include "PieceInterface.h"

class MockPiece : public PieceInterface
{
public:
    MockPiece() : PieceInterface() {}
    virtual ~MockPiece() = default;

public:
    int toValue() override { return 0; }

    int getType() override { return 0; }
    int getPosX() override { return 0; }
    int getPosY() override { return 0; }
    int getRotation() override { return 0; }

    int getCellCount() override { return 0; }
    int getRotationCount() override { return 0; }
    int getCellPosX(int) override { return 0; }
    int getCellPosY(int) override { return 0; }
};

#endif //MOCKPIECE_H
