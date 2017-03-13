/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   Copyright (C) 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
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
