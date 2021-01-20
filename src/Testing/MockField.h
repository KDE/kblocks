/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/
#ifndef MOCKFIELD_H
#define MOCKFIELD_H

#include "FieldInterface.h"

class MockField : public FieldInterface
{
public:
    MockField() : FieldInterface() {}
    virtual ~MockField() = default;

public:
    bool getCell(int, int) override { return false; }

    int getWidth() override { return 0; }
    int getHeight() override { return 0; }
};

#endif //MOCKFIELD_H
