#include "KBlocksLayout.h"

KBlocksLayout::KBlocksLayout()
{
    mpGameField = 0;
    mpActivePiece = 0;
    mpNextPiece = 0;
    
    mpLastPiecePos = new QPoint*[4]();
    for(int i = 0; i < 4; i++)
    {
        mpLastPiecePos[i] = new QPoint(0, 0);
    }
}

KBlocksLayout::~KBlocksLayout()
{
    for(int i = 0; i < 4; i++)
    {
        delete mpLastPiecePos[i];
    }
    delete [] mpLastPiecePos;
}

void KBlocksLayout::setGameField(FieldInterface * p)
{
    mpGameField = p;
}

void KBlocksLayout::setActivePiece(PieceInterface * p)
{
    mpActivePiece = p;
}

void KBlocksLayout::setNextPiece(PieceInterface * p)
{
    mpNextPiece = p;
}

void KBlocksLayout::initLayout()
{
    int width = mpGameField->getWidth();
    int height = mpGameField->getHeight();
    boardInfo = new int*[height]();
    for(int i = 0; i < height; i++)
    {
        boardInfo[i] = new int[width];
        for(int j = 0; j < width; j++)
        {
            boardInfo[i][j] = -1;
        }
    }
    
    prepareInfo = new int*[PREPARE_AREA_WIDTH]();
    for(int i = 0; i < PREPARE_AREA_WIDTH; i++)
    {
        prepareInfo[i] = new int[PREPARE_AREA_WIDTH];
        for(int j = 0; j < PREPARE_AREA_WIDTH; j++)
        {
            prepareInfo[i][j] = -1;
        }
    }
}

void KBlocksLayout::freeLayout()
{
    int height = mpGameField->getHeight();
    for(int i = 0; i < height; i++)
    {
        delete boardInfo[i];
    }
    delete [] boardInfo;
    
    for(int i = 0; i < PREPARE_AREA_WIDTH; i++)
    {
        delete prepareInfo[i];
    }
    delete [] prepareInfo;
}

void KBlocksLayout::updateLayout(int lineCount, int * lineList)
{
    if ((!boardInfo) or (!prepareInfo))
    {
        return;
    }
    
    if ((!mpGameField) or (!mpActivePiece) or (!mpNextPiece))
    {
        return;
    }
    
    int width = mpGameField->getWidth();
    int height = mpGameField->getHeight();
    
    if (lineCount == -1)
    {
        for(int i = 0; i < 4; i++)
        {
            int posX = mpLastPiecePos[i]->x();
            int posY = mpLastPiecePos[i]->y();
            if ((posX >= 0) && (posX < width)
             && (posY >= 0) && (posY < height))
            {
                boardInfo[posY][posX] = -1;
            }
        }
    }
    else if (lineCount > 0)
    {
        for(int k = lineCount - 1; k >= 0; k--)
        {
            for(int i = lineList[k]; i > 0; i--)
            {
                for(int j = 0; j < width; j++)
                {
                    boardInfo[i][j] = boardInfo[i - 1][j];
                }
            }
            for(int j = 0; j < width; j++)
            {
                boardInfo[0][j] = -1;
            }
        }
    }
    
    int activePieceColor = mpActivePiece->getType();
    for(int i = 0; i < 4; i++)
    {
        int posX = mpActivePiece->getCellPosX(i);
        int posY = mpActivePiece->getCellPosY(i);
        mpLastPiecePos[i]->setX(posX);
        mpLastPiecePos[i]->setY(posY);
        if ((posX >= 0) && (posX < width) && (posY >= 0) && (posY < height))
        {
            boardInfo[posY][posX] = activePieceColor;
        }
    }
    
    for(int i = 0; i < PREPARE_AREA_WIDTH; i++)
    {
        for(int j = 0; j < PREPARE_AREA_WIDTH; j++)
        {
            prepareInfo[i][j] = -1;
        }
    }
    
    int nextPieceColor = mpNextPiece->getType();
    for(int i = 0; i < 4; i++)
    {
        int posX = mpNextPiece->getCellPosX(i);
        int posY = mpNextPiece->getCellPosY(i);
        prepareInfo[posY][posX] = nextPieceColor;
    }
}

int KBlocksLayout::getFieldColor(int posX, int posY)
{
    if (!boardInfo)
    {
        return -1;
    }
    
    return boardInfo[posY][posX];
}

int KBlocksLayout::getPrepareColor(int posX, int posY)
{
    if (!prepareInfo)
    {
        return -1;
    }
    
    return prepareInfo[posY][posX];
}
