/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   Copyright (C) 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
***************************************************************************/
#include "KBlocksAIPlanner.h"
#include "KBlocksAILog.h"

/* ############################################################
#####   Definition    #########################################
############################################################ */
struct Span {
    Span() : Span(0,0)
    {
    }
    Span(int mn, int mx)
    {
        min = mn;
        max = mx;
    }
    int min;
    int max;
};

typedef std::vector<int> Lines;
typedef std::vector<Span> Spans;

/* ############################################################
#####   KBlocksAIPlanner    ###################################
############################################################ */
KBlocksAIPlanner::KBlocksAIPlanner(KBlocksField *p) : PlannerInterface(p)
{
    mNextPieceValues.clear();
}

KBlocksAIPlanner::~KBlocksAIPlanner()
{
    mNextPieceValues.clear();
}

int KBlocksAIPlanner::process(KBlocks_PieceType_Detail pieceValue)
{
    // board size
    int w = mpField->getWidth();
    int h = mpField->getHeight();
    // piece bound
    Spans p_Col_Height = Spans(w);
    // board rect
    Lines b_Col_maxHeight = Lines(w);

    // board info - max height per column
    for (int x = 0; x < w; x++) {
        int y = 0;
        for (y = 0; y < h; y++) {
            if (mpField->getCell(x, y)) {
                break;
            }
        }
        b_Col_maxHeight[x] = y;
    }

    // init next_states list
    mNextPieceValues.clear();

    // init piece state
    KBlocksPiece piece;
    piece.fromValue(pieceValue);

    int loopCount = piece.getRotationCount();
    // scan all possible rotation
    for (int rotation = 0; rotation < loopCount; rotation++) {
        // piece info - min/max height per column
        piece.setRotation(rotation);

        // scan all possible x position - put piece on board
        for (int x = 0; x < w; x++) {
            bool invalidPos = false;
            piece.setPosX(x);
            piece.setPosY(0);

            // init
            for (int i = 0; i < w; i++) {
                p_Col_Height[i] = Span(h, -1);
            }
            for (int i = 0; i < KBlocksPiece_CellCount; i++) {
                int cx = piece.getCellPosX(i);
                int cy = piece.getCellPosY(i);
                if (cx < 0 || cx >= w || mpField->getCell(cx, cy)) {
                    invalidPos = true;
                    break;
                }
                if (p_Col_Height[cx].min > cy) {
                    p_Col_Height[cx].min = cy;
                }
                if (p_Col_Height[cx].max < cy) {
                    p_Col_Height[cx].max = cy;
                }
            }
            if (invalidPos) {
                continue;
            }

            // get response height
            int y = h;
            for (int px = 0; px < w; px++) {
                if (p_Col_Height[px].min == h) {
                    continue;
                }
                int dy = b_Col_maxHeight[px] - 1 - p_Col_Height[px].max;
                if (dy < y) {
                    y = dy;
                }
            }

            if ((y < 0) || (y >= h)) {
                continue;
            }

            // state
            piece.setPosY(y);
            // insert to list
            mNextPieceValues.push_back(piece);
        }
    }

    return (int)(mNextPieceValues.size());
}

bool KBlocksAIPlanner::getNextBoardStatus(int index, KBlocksField *field)
{
    KBlocksPiece *piece = new KBlocksPiece();

    if (!getNextPieceState(index, piece)) {
        delete field;
        field = 0;
        delete piece;
        return false;
    }

    field->copy(mpField);

    for (int i = 0; i < KBlocksPiece_CellCount; i++) {
        field->setCell(piece->getCellPosX(i), piece->getCellPosY(i), true);
    }
    int maxLines = field->getHeight();
    for (int i = 0; i < maxLines; i++) {
        if (field->checkFilledLine(i)) {
            field->removeFilledLine(i);
        }
    }

    delete piece;

    return true;
}

bool KBlocksAIPlanner::getNextPieceState(int index, KBlocksPiece *piece)
{
    if ((index >= (int)mNextPieceValues.size()) || (index < 0)) {
        return false;
    }
    piece->copy(&mNextPieceValues[index]);
    return true;
}

int KBlocksAIPlanner::count()
{
    return mNextPieceValues.size();
}
