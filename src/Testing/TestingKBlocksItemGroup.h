/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/

#ifndef TESTINGKBLOCKSITEMGROUP_H
#define TESTINGKBLOCKSITEMGROUP_H

#include "KBlocksItemGroup.h"

class TestingKBlocksItemGroup : public KBlocksItemGroup
{
public:
    TestingKBlocksItemGroup(int groupID, SingleGameInterface *p, GraphicsInterface *pG, SoundInterface *pS)
        : KBlocksItemGroup(groupID, p, pG, pS)
    {}
    virtual ~TestingKBlocksItemGroup() = default;

    void callUpdateGame()
    {
        KBlocksItemGroup::updateGame();
    }
};

#endif // TESTINGKBLOCKSITEMGROUP_H
