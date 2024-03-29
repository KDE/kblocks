/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#ifndef KBLOCKSLAYOUT_H
#define KBLOCKSLAYOUT_H

#include "FieldInterface.h"
#include "PieceInterface.h"

#include "KBlocksDefine.h"

#include <QPoint>
#include <QList>

enum KBlocks_Layout_Update_Type {
    KBlocksLayout_Update_FreezePiece = 0,
    KBlocksLayout_Update_RemoveLine,
    KBlocksLayout_Update_PunishLine,
    KBlocksLayout_Update_Max_Count
};

class KBlocksLayout
{
public:
    KBlocksLayout(FieldInterface *pF, PieceInterface *pA, PieceInterface *pN);
    ~KBlocksLayout();

    void beginUpdate(QList<int> *list);
    void updateLayout(int type, const QList<int> &dataList);
    void endUpdate();

    void updateSnapshot();

    int getFieldColor(int posX, int posY);
    int getPrepareColor(int posX, int posY);

private:
    void updatePrepareArea();
    void updateFreezePiece(const QList<int> &dataList);
    void updateRemoveLine(const QList<int> &dataList);
    void updatePunishLine(const QList<int> &dataList);

private:
    FieldInterface *mpGameField = nullptr;
    PieceInterface *mpActivePiece = nullptr;
    PieceInterface *mpNextPiece = nullptr;

    int **boardInfo;
    int **prepareInfo;

    int mPieceCellCount;
    QPoint **mpLastPiecePos;

    int mWidth;
    int mHeight;
};

#endif

