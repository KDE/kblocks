/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   SPDX-FileCopyrightText: 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   SPDX-License-Identifier: GPL-2.0-or-later
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
    ~KBlocksAIPlanner() override;

    int  process(KBlocks_PieceType_Detail pieceValue) override;
    bool getNextBoardStatus(int index, KBlocksField *field) override;
    bool getNextPieceState(int index, KBlocksPiece *piece) override;

    int count() override;

private:
    AIPlanner_PieceInfo_Sequence mNextPieceValues;
};

#endif
