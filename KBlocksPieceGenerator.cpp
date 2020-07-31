/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksPieceGenerator.h"

#include <QtGlobal>
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

