#include <stdlib.h>

#include "KBlocksPieceGenerator.h"

#define  KBLOCKS_PIECE_TYPE_NUM    28

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
        maPieceList[i] = rand() % KBLOCKS_PIECE_TYPE_NUM;
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
