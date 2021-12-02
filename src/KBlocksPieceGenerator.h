/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
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

