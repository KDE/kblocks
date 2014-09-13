/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2010 University Freiburg                                *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
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
    explicit KBlocksAIPlanner(KBlocksField * p);
    ~KBlocksAIPlanner();
    
    int  process(KBlocks_PieceType_Detail pieceValue);
    bool getNextBoardStatus(int index, KBlocksField * field);
    bool getNextPieceState(int index, KBlocksPiece * piece);

    virtual int count();
    
private:      
    AIPlanner_PieceInfo_Sequence mNextPieceValues;
};

#endif
