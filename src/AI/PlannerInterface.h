/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   SPDX-FileCopyrightText: 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#ifndef PLANNERINTERFACE_H
#define PLANNERINTERFACE_H

#include "KBlocksAITypeDefine.h"

#include "../KBlocksField.h"
#include "../KBlocksPiece.h"

class PlannerInterface
{
public:
    explicit PlannerInterface(KBlocksField *p)
    {
        mpField = p;
    };
    virtual ~PlannerInterface() {};

    virtual int  process(KBlocks_PieceType_Detail) = 0;

    virtual bool getNextBoardStatus(int, KBlocksField *) = 0;
    virtual bool getNextPieceState(int, KBlocksPiece *) = 0;

    virtual int  count() = 0;

protected:
    KBlocksField *mpField;
};

#endif
