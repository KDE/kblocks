/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2010 University Freiburg                                *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSAIPLANNEREXTEND_H
#define KBLOCKSAIPLANNEREXTEND_H

#include "KBlocksAIPlanner.h"
#include <list>
#include <vector>

/****************************************************************
**  Planning Path  **********************************************
****************************************************************/
namespace PlanningPath
{
    class PathNode
    {
        public:
            PathNode(KBlocksPiece * piece);
            ~PathNode();
            
            KBlocksPiece getContent();
            void setContent(KBlocksPiece * piece);
            void setParent(PathNode *parent);
            PathNode *getParent();
            
            void setNext(std::vector<PathNode>* &);
            
        private:
            KBlocksPiece content;
            PathNode *parent;
            std::vector<PathNode> *next;
    };
    
    typedef std::vector<PathNode>  PathTree;
    typedef std::vector<PathNode*> LeafList;  
}

/****************************************************************
**  Planner  ****************************************************
****************************************************************/
class KBlocksAIPlannerExtend : public KBlocksAIPlanner
{
public:
    KBlocksAIPlannerExtend(KBlocksField * field);
    ~KBlocksAIPlannerExtend();
    
    int process(KBlocks_PieceType_Detail pieceValue);
    int process(const AIPlanner_PieceValue_Sequence &p);
    
    bool getNextBoardStatus(int index, KBlocksField* field);
    bool getNextBoardStatus(int index, KBlocksField* field, bool first);
    bool getNextPieceState(int index, KBlocksPiece* piece);
    bool getPath(int index, AIPlanner_PieceInfo_Sequence * pseq);
    
    int count();
    
private:
    PlanningPath::PathTree *mPathTree;
    PlanningPath::LeafList *mPathList;
    void process_nstep_recursive( PlanningPath::PathNode* parent, 
                                  AIPlanner_PieceValue_Sequence &pseq,
                                  PlanningPath::PathTree &tree,
                                  PlanningPath::LeafList &leaf);
};

#endif
