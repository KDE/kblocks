/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   Copyright (C) 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
***************************************************************************/
#ifndef KBLOCKSAIPLANNER_H
#define KBLOCKSAIPLANNER_H

#include <list>
#include <vector>

#include "PlannerInterface.h"

typedef std::list<KBlocks_PieceType_Detail> AIPlanner_PieceValue_Sequence;
typedef std::vector<KBlocksPiece> AIPlanner_PieceInfo_Sequence;

class KBlocksAIPlanner : public PlannerInterface
{
public:
    explicit KBlocksAIPlanner(KBlocksField *p);
    ~KBlocksAIPlanner();

    int  process(KBlocks_PieceType_Detail pieceValue) override;
    bool getNextBoardStatus(int index, KBlocksField *field) override;
    bool getNextPieceState(int index, KBlocksPiece *piece) override;

    int count() override;

private:
    AIPlanner_PieceInfo_Sequence mNextPieceValues;
};

#endif
