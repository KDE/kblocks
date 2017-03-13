/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   Copyright (C) 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
***************************************************************************/
#include "KBlocksAIPlannerExtend.h"
#include "KBlocksAILog.h"

using namespace PlanningPath;

/********************************************************************************
**** Data Structure - PathNode *************************************************
********************************************************************************/
PathNode::PathNode(KBlocksPiece *piece)
{
    parent = 0;
    next = 0;
    setContent(piece);
}

PathNode::~PathNode()
{
    if (next == 0) {
        return;
    }
    next->clear();
    delete next;
}

void PathNode::setContent(KBlocksPiece *piece)
{
    content = *piece;
}

KBlocksPiece PathNode::getContent()
{
    return content;
}

void PathNode::setParent(PathNode *parent)
{
    this->parent = parent;
}

PathNode *PathNode::getParent()
{
    return parent;
}

void PathNode::setNext(PathTree *&next)
{
    this->next = next;
}

/*******************************************************************************
**** Planner  ******************************************************************
********************************************************************************/
KBlocksAIPlannerExtend::KBlocksAIPlannerExtend(KBlocksField *field) : KBlocksAIPlanner(field)
{
    mPathTree = new PathTree();
    mPathList = new LeafList();
}

KBlocksAIPlannerExtend::~KBlocksAIPlannerExtend()
{
    mPathTree->clear();
    delete mPathTree;
    mPathList->clear();
    delete mPathList;
}

bool KBlocksAIPlannerExtend::getPath(int index, AIPlanner_PieceInfo_Sequence *pseq)
{
    if ((mPathList == 0) || (index >= (int)mPathList->size())) {
        return false;
    }

    PathNode *node  = (*mPathList)[index];
    while (node != 0) {
        KBlocksPiece piece = node->getContent();
        pseq->push_back(piece);
        node = node->getParent();
    }

    return true;
}

bool KBlocksAIPlannerExtend::getNextBoardStatus(int index, KBlocksField *field)
{
    return getNextBoardStatus(index, field, false);
}

bool KBlocksAIPlannerExtend::getNextBoardStatus(int index, KBlocksField *field, bool first)
{
    AIPlanner_PieceInfo_Sequence path = AIPlanner_PieceInfo_Sequence(0);

    if (getPath(index, &path) == false) {
        delete field;
        field = 0;
        return false;
    }

    if (first) {
        KBlocksPiece firstps = path.back();
        path.clear();
        path.push_back(firstps);
    }

    field->copy(mpField);

    for (int i = (int)path.size() - 1; i >= 0; i--) {
        KBlocksPiece *piece = &path[i];
        for (int j = 0; j < KBlocksPiece_CellCount; j++) {
            field->setCell(piece->getCellPosX(j), piece->getCellPosY(j), true);
        }
        int maxLines = field->getHeight();
        for (int i = 0; i < maxLines; i++) {
            if (field->checkFilledLine(i)) {
                field->removeFilledLine(i);
            }
        }
    }

    return true;
}

bool KBlocksAIPlannerExtend::getNextPieceState(int index, KBlocksPiece *piece)
{
    if (index >= (int)mPathList->size()) {
        return false;
    }
    PathNode *node = (*mPathList)[index];
    PathNode *last = 0;
    while (node != 0) {
        last = node;
        node = node->getParent();
    }
    if (last == 0) {
        return false;
    }
    *piece = last->getContent();
    return true;
}

int KBlocksAIPlannerExtend::count()
{
    return mPathList->size();
}

// get next piece_state
int KBlocksAIPlannerExtend::process(KBlocks_PieceType_Detail pieceValue)
{
    AIPlanner_PieceValue_Sequence pseq = AIPlanner_PieceValue_Sequence(0);
    pseq.push_front(pieceValue);
    return process(pseq);
}

// get next piece_state
int KBlocksAIPlannerExtend::process(const AIPlanner_PieceValue_Sequence &pseq)
{
    mPathTree->clear();
    mPathList->clear();
    PathNode *parent = 0;
    AIPlanner_PieceValue_Sequence tmpPSeq = AIPlanner_PieceValue_Sequence(pseq);
    process_nstep_recursive(parent, tmpPSeq, *mPathTree, *mPathList);
    return mPathList->size();
}

void KBlocksAIPlannerExtend::process_nstep_recursive(PlanningPath::PathNode *parent,
        AIPlanner_PieceValue_Sequence &pseq,
        PlanningPath::PathTree &tree,
        PlanningPath::LeafList &leaf)
{
    if (pseq.size() == 0) {
        return;
    }

    bool isLeaf = (pseq.size() == 1);

    KBlocks_PieceType_Detail first_piece_type = pseq.front();
    pseq.pop_front();

    int n = KBlocksAIPlanner::process(first_piece_type);
    for (int i = 0; i < n; i++) {
        KBlocksPiece piece;
        KBlocksAIPlanner::getNextPieceState(i, &piece);
        tree.push_back(PathNode(&piece));
    }

    for (int i = 0; i < (int)tree.size(); i++) {
        // get next possible piece state
        PathNode *_node = &tree[i];
        _node->setParent(parent);
        KBlocksPiece _pstate = tree[i].getContent();

        // get current board_status based on current piece state
        KBlocksField *_tmpBS = new KBlocksField(mpField);
        for (int i = 0; i < KBlocksPiece_CellCount; i++) {
            _tmpBS->setCell(_pstate.getCellPosX(i), _pstate.getCellPosY(i), true);
        }
        // remove the filled lines in current board
        int maxLines = _tmpBS->getHeight();
        for (int i = 0; i < maxLines; i++) {
            if (_tmpBS->checkFilledLine(i)) {
                _tmpBS->removeFilledLine(i);
            }
        }

        // get copy of piece_info_sequence
        AIPlanner_PieceValue_Sequence _pseq = AIPlanner_PieceValue_Sequence(pseq);

        // get next tree based on _tmpBS and _pseq
        KBlocksAIPlannerExtend planner = KBlocksAIPlannerExtend(_tmpBS);
        PathTree *_next = new PathTree();
        planner.process_nstep_recursive(_node, _pseq, *_next, leaf);
        _node->setNext(_next);

        if (isLeaf) {
            leaf.push_back(_node);
        }

        delete _tmpBS;
    }
}
