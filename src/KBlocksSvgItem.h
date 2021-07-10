/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2010-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/
#ifndef KBLOCKSSVGITEM_H
#define KBLOCKSSVGITEM_H

#include <QGraphicsPixmapItem>

#include "KBlocksLayout.h"
#include "SvgItemInterface.h"

enum KBlocks_SvgItem_Type {
    KBlocksSvgItem_None = -1,
    KBlocksSvgItem_FieldArea = 0,
    KBlocksSvgItem_PrepareArea,
    KBlocksSvgItem_Max_Count
};

class KBlocksSvgItem : public SvgItemInterface
{
    Q_OBJECT

public:
    KBlocksSvgItem(KBlocksLayout *p, int type, int posX, int posY);
    virtual ~KBlocksSvgItem();

    void setLayoutPos(int posX, int posY);

    bool updateSelf() override;
    void clearCache() override;

    void startOpAnim() override;
    void stopOpAnim() override;

    void startPosAnim(QPointF target) override;
    void execPosAnim(qreal step) override;
    void stopPosAnim() override;

private:
    KBlocksLayout *mpGameLayout;

    int mPosX;
    int mPosY;

    int mType;
    int mColor;

    QPointF mOriginPos;
    QPointF mTargetPos;
};

#endif

