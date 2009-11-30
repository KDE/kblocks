#include "KBlocksControl.h"

KBlocksControl::KBlocksControl()
{
    mpSingleGame = 0;
}

KBlocksControl::~KBlocksControl()
{
}

void KBlocksControl::setSingleGame(SingleGameInterface * p)
{
    mpSingleGame = p;
}

bool KBlocksControl::rotateCW()
{
    return mpSingleGame->setCurrentPiece(0, 0, 1);
}

bool KBlocksControl::rotateCCW()
{
    return mpSingleGame->setCurrentPiece(0, 0, -1);
}

bool KBlocksControl::moveLeft()
{
    return mpSingleGame->setCurrentPiece(-1, 0, 0);
}

bool KBlocksControl::moveRight()
{
    return mpSingleGame->setCurrentPiece(1, 0, 0);
}

bool KBlocksControl::moveDown()
{
    return mpSingleGame->setCurrentPiece(0, 1, 0);
}

bool KBlocksControl::pushDown()
{
    while(mpSingleGame->setCurrentPiece(0, 1, 0)) ;
    //mpSingleGame->runOneStep(0);
    return true;
}

int KBlocksControl::getRemovedLines(int * lineList)
{
    int lineCount = 0;
    int lineIndex = 0;
    
    while(mpSingleGame->popGameAction(GameAction_Remove_Line, &lineIndex))
    {
        lineList[lineCount] = lineIndex;
        lineCount++;
    }
    
    return lineCount;
}

int KBlocksControl::getRemovedCount()
{
    int lineCount = 0;
    mpSingleGame->popGameAction(GameAction_Remove_Count, &lineCount);
    return lineCount;
}

int KBlocksControl::getNewPiecePosX(int * posX)
{
    int pieceCount = 0;
    int tmpPosX = 0;
    while(mpSingleGame->popGameAction(GameAction_New_Piece_X, &tmpPosX))
    {
        posX[pieceCount] = tmpPosX;
        pieceCount++;
    }
    return pieceCount;
}

int KBlocksControl::getNewPiecePosY(int * posY)
{
    int pieceCount = 0;
    int tmpPosY = 0;
    while(mpSingleGame->popGameAction(GameAction_New_Piece_Y, &tmpPosY))
    {
        posY[pieceCount] = tmpPosY;
        pieceCount++;
    }
    return pieceCount;
}
