/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   SPDX-FileCopyrightText: 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#include "KBlocksAIPlannerExtend.h"
#include "KBlocksAILog.h"

using namespace PlanningPath;

/********************************************************************************
**** Data Structure - PathNode *************************************************
********************************************************************************/
PathNode::PathNode(KBlocksPiece *piece)
{
    parent = nullptr;
    next = nullptr;
    setContent(piece);
}

PathNode::~PathNode()
{
    if (next == nullptr) {
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
    if ((mPathList == nullptr) || (index >= (int)mPathList->size())) {
        return false;
    }

    PathNode *node  = (*mPathList)[index];
    while (node != nullptr) {
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
        field = nullptr;
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
        for (int lineI = 0; lineI < maxLines; lineI++) {
            if (field->checkFilledLine(lineI)) {
                field->removeFilledLine(lineI);
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
    PathNode *last = nullptr;
    while (node != nullptr) {
        last = node;
        node = node->getParent();
    }
    if (last == nullptr) {
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
    PathNode *parent = nullptr;
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
        for (int cellI = 0; cellI < KBlocksPiece_CellCount; cellI++) {
            _tmpBS->setCell(_pstate.getCellPosX(cellI), _pstate.getCellPosY(cellI), true);
        }
        // remove the filled lines in current board
        int maxLines = _tmpBS->getHeight();
        for (int lineI = 0; lineI < maxLines; lineI++) {
            if (_tmpBS->checkFilledLine(lineI)) {
                _tmpBS->removeFilledLine(lineI);
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
