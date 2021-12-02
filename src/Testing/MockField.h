/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#ifndef MOCKFIELD_H
#define MOCKFIELD_H

#include "FieldInterface.h"

class MockField : public FieldInterface
{
public:
    MockField() : FieldInterface() {}
    ~MockField() override = default;

public:
    bool getCell(int, int) override { return false; }

    int getWidth() override { return 0; }
    int getHeight() override { return 0; }
};

#endif //MOCKFIELD_H
