/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksLayout.h"

KBlocksLayout::KBlocksLayout(FieldInterface *pF, PieceInterface *pA, PieceInterface *pN)
{
    mpGameField = pF;
    mpActivePiece = pA;
    mpNextPiece = pN;

    mPieceCellCount = mpActivePiece->getCellCount();
    mpLastPiecePos = new QPoint*[mPieceCellCount];
    for (int i = 0; i < mPieceCellCount; i++) {
        mpLastPiecePos[i] = new QPoint(0, 0);
    }

    mWidth = mpGameField->getWidth();
    mHeight = mpGameField->getHeight();
    boardInfo = new int *[mHeight];
    for (int i = 0; i < mHeight; i++) {
        boardInfo[i] = new int[mWidth];
        for (int j = 0; j < mWidth; j++) {
            boardInfo[i][j] = -1;
        }
    }

    prepareInfo = new int *[PREPARE_AREA_WIDTH];
    for (int i = 0; i < PREPARE_AREA_WIDTH; i++) {
        prepareInfo[i] = new int[PREPARE_AREA_WIDTH];
        for (int j = 0; j < PREPARE_AREA_WIDTH; j++) {
            prepareInfo[i][j] = -1;
        }
    }
}

KBlocksLayout::~KBlocksLayout()
{
    for (int i = 0; i < mHeight; i++) {
        delete [] boardInfo[i];
    }
    delete [] boardInfo;

    for (int i = 0; i < PREPARE_AREA_WIDTH; i++) {
        delete [] prepareInfo[i];
    }
    delete [] prepareInfo;

    for (int i = 0; i < mPieceCellCount; i++) {
        delete mpLastPiecePos[i];
    }
    delete [] mpLastPiecePos;
}

void KBlocksLayout::beginUpdate(QList<int> *list)
{
    int px = 0;
    int py = 0;
    list->clear();
    for (int i = 0; i < mPieceCellCount; i++) {
        px = mpLastPiecePos[i]->x();
        py = mpLastPiecePos[i]->y();
        list->append(px);
        list->append(py);
        if ((px >= 0) && (px < mWidth)
                && (py >= 0) && (py < mHeight)) {
            boardInfo[py][px] = -1;
        }
    }
}

void KBlocksLayout::updateLayout(int type, const QList<int> &dataList)
{
    switch (type) {
    case KBlocksLayout_Update_FreezePiece:
        updateFreezePiece(dataList);
        break;
    case KBlocksLayout_Update_RemoveLine:
        updateRemoveLine(dataList);
        break;
    case KBlocksLayout_Update_PunishLine:
        updatePunishLine(dataList);
        break;
    }
}

void KBlocksLayout::endUpdate()
{
    int px = 0;
    int py = 0;
    int activePieceColor = mpActivePiece->getType();
    for (int i = 0; i < mPieceCellCount; i++) {
        px = mpActivePiece->getCellPosX(i);
        py = mpActivePiece->getCellPosY(i);
        mpLastPiecePos[i]->setX(px);
        mpLastPiecePos[i]->setY(py);
        if ((px >= 0) && (px < mWidth)
                && (py >= 0) && (py < mHeight)) {
            boardInfo[py][px] = activePieceColor;
        }
    }

    updatePrepareArea();
}

void KBlocksLayout::updateSnapshot()
{
    for (int y = 0; y < mHeight; y++) {
        for (int x = 0; x < mWidth; x++) {
            if (mpGameField->getCell(x, y)) {
                boardInfo[y][x] = 0; // TODO : what color is better here?
            } else {
                boardInfo[y][x] = -1;
            }
        }
    }

    endUpdate();
}

int KBlocksLayout::getFieldColor(int posX, int posY)
{
    if ((posX < 0) || (posX >= mWidth) || (posY < 0) || (posY >= mHeight)) {
        return -1;
    }

    return boardInfo[posY][posX];
}

int KBlocksLayout::getPrepareColor(int posX, int posY)
{
    if ((posX < 0) || (posX >= PREPARE_AREA_WIDTH) || (posY < 0) || (posY >= PREPARE_AREA_WIDTH)) {
        return -1;
    }

    return prepareInfo[posY][posX];
}

void KBlocksLayout::updatePrepareArea()
{
    for (int i = 0; i < PREPARE_AREA_WIDTH; i++) {
        for (int j = 0; j < PREPARE_AREA_WIDTH; j++) {
            prepareInfo[i][j] = -1;
        }
    }

    int nextPieceColor = mpNextPiece->getType();
    for (int i = 0; i < 4; i++) {
        int posX = mpNextPiece->getCellPosX(i);
        int posY = mpNextPiece->getCellPosY(i);
        prepareInfo[posY][posX] = nextPieceColor;
    }
}

void KBlocksLayout::updateFreezePiece(const QList<int> &dataList)
{
    QList<int> tmpList = dataList;
    int freezeColor = tmpList.takeFirst();
    int loopCount = tmpList.size() / 2;
    for (int i = 0; i < loopCount; i++) {
        int posX = tmpList[i * 2];
        int posY = tmpList[i * 2 + 1];
        if ((posX >= 0) && (posX < mWidth)
                && (posY >= 0) && (posY < mHeight)) {
            boardInfo[posY][posX] = freezeColor;
        }
    }
}

void KBlocksLayout::updateRemoveLine(const QList<int> &dataList)
{
    int lineCount = dataList.size();
    for (int k = 0; k < lineCount; k++) {
        for (int i = dataList[k]; i > 0; i--) {
            for (int j = 0; j < mWidth; j++) {
                boardInfo[i][j] = boardInfo[i - 1][j];
            }
        }
        for (int j = 0; j < mWidth; j++) {
            boardInfo[0][j] = -1;
        }
    }
}

void KBlocksLayout::updatePunishLine(const QList<int> &dataList)
{
    int punishColor = mpNextPiece->getType(); // TODO : this color or new color?
    int lineCount = dataList.size();
    for (int k = 0; k < lineCount; k++) {
        for (int i = 0; i < mHeight - dataList[k]; i++) {
            for (int j = 0; j < mWidth; j++) {
                boardInfo[i][j] = boardInfo[i + dataList[k]][j];
            }
        }
        for (int i = mHeight - dataList[k]; i < mHeight; i++) {
            for (int j = 0; j < mWidth; j++) {
                if (mpGameField->getCell(j, i)) {
                    boardInfo[i][j] = punishColor;
                } else {
                    boardInfo[i][j] = -1;
                }
            }
        }
    }
}
