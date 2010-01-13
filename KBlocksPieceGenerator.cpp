/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include <stdlib.h>

#include "KBlocksPieceGenerator.h"
#include "KBlocksPiece.h"

KBlocksPieceGenerator::KBlocksPieceGenerator(int size)
{
    maxCapacity = size;
    pieceIndex = 0;
    maPieceList = new int[maxCapacity]();
}

KBlocksPieceGenerator::~KBlocksPieceGenerator()
{
    delete [] maPieceList;
}

void KBlocksPieceGenerator::genList(int seed)
{
    srand(seed);
    
    for(int i = 0; i < maxCapacity; i++)
    {
        maPieceList[i] = rand() % PieceType_Detail_Max_Count;
    }
    
    pieceIndex = 0;
}

int KBlocksPieceGenerator::getPiece()
{
    pieceIndex++;
    
    if (pieceIndex > maxCapacity)
    {
        pieceIndex = 0;
    }
    
    return maPieceList[pieceIndex];
}

int KBlocksPieceGenerator::getIndex()
{
    return pieceIndex;
}

