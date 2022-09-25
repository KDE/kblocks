/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#include "KBlocksPieceGenerator.h"

#include <QRandomGenerator>
#include <stdio.h>

#include "KBlocksPiece.h"

KBlocksPieceGenerator::KBlocksPieceGenerator(int size)
{
    maxCapacity = size;
    pieceIndex = 0;
    maPieceList = new int[maxCapacity];
}

KBlocksPieceGenerator::~KBlocksPieceGenerator()
{
    delete [] maPieceList;
}

void KBlocksPieceGenerator::genList(int seed)
{
    auto random = QRandomGenerator::global();
    for (int i = 0; i < maxCapacity; i++) {
        maPieceList[i] = random->bounded(PieceType_Detail_Max_Count);
    }

    pieceIndex = 0;
}

int KBlocksPieceGenerator::getPiece()
{
    pieceIndex++;

    if (pieceIndex > maxCapacity) {
        pieceIndex = 0;
        genList(maPieceList[0]);
    }

    return maPieceList[pieceIndex];
}

int KBlocksPieceGenerator::getIndex()
{
    return pieceIndex;
}

