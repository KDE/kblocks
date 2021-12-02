/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#ifndef MOCKPIECE_H
#define MOCKPIECE_H

#include "PieceInterface.h"

class MockPiece : public PieceInterface
{
public:
    MockPiece() : PieceInterface() {}
    ~MockPiece() override = default;

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
