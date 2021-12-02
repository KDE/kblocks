/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/

#ifndef TESTINGKBLOCKSITEMGROUP_H
#define TESTINGKBLOCKSITEMGROUP_H

#include "KBlocksItemGroup.h"
#include "SvgItemInterface.h"

class TestingKBlocksItemGroup : public KBlocksItemGroup
{
public:
    TestingKBlocksItemGroup(int groupID, SingleGameInterface *p, GraphicsInterface *pG, SoundInterface *pS)
        : KBlocksItemGroup(groupID, p, pG, pS)
    {}
    ~TestingKBlocksItemGroup() override = default;

    void callUpdateGame()
    {
        KBlocksItemGroup::updateGame();
    }

    SvgItemInterface *getFreezeCell(int cellNum)
    {
        if (cellNum < mMaxFreezeCellNum) {
            return maFreezeCells[cellNum];
        }
        return nullptr;
    }

    int getMaxPrepareCellNum() const
    {
        return mMaxPrepareCellNum;
    }

    SvgItemInterface *getPrepareCell(int cellNum)
    {
        if (cellNum < mMaxPrepareCellNum) {
            return maPrepareCells[cellNum];
        }
        return nullptr;
    }

    void replacePrepareCell(int cellNum, SvgItemInterface* newItem)
    {
        if (cellNum >= mMaxPrepareCellNum) {
            return;
        }
        removeFromGroup(maPrepareCells[cellNum]);
        delete maPrepareCells[cellNum];

        maPrepareCells[cellNum] = newItem;
        addToGroup(maPrepareCells[cellNum]);
    }

    void replaceFreezeCell(int cellNum, SvgItemInterface* newItem)
    {
        if (cellNum >= mMaxFreezeCellNum) {
            return;
        }
        removeFromGroup(maFreezeCells[cellNum]);
        delete maFreezeCells[cellNum];

        maFreezeCells[cellNum] = newItem;
        addToGroup(maFreezeCells[cellNum]);
    }
};

#endif // TESTINGKBLOCKSITEMGROUP_H
