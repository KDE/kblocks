/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSPIECEGENERATOR_H
#define KBLOCKSPIECEGENERATOR_H

class KBlocksPieceGenerator
{
public:
    explicit KBlocksPieceGenerator(int size = 4096);
    ~KBlocksPieceGenerator();

public:
    void genList(int seed);
    int getPiece();
    int getIndex();

private:
    int  maxCapacity;
    int  pieceIndex;
    int *maPieceList = nullptr;
};

#endif

