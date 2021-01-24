/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2009-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/
#include "KBlocksAnimFade.h"

#include "SvgItemInterface.h"

KBlocksAnimFade::KBlocksAnimFade(const QList<SvgItemInterface *> &items, int duration, QTimeLine::Direction direction)
{
    mItemList = items;

    mpTimeLine = new QTimeLine(duration);
    mpTimeLine->setFrameRange(0, 30);
    mpTimeLine->setDirection(direction);

    connect(mpTimeLine, &QTimeLine::valueChanged, this, &KBlocksAnimFade::valueChanged);
    connect(mpTimeLine, &QTimeLine::finished, this, &KBlocksAnimFade::endAnimation);

    mpTimeLine->start();
}

KBlocksAnimFade::~KBlocksAnimFade()
{
    delete mpTimeLine;
}

void KBlocksAnimFade::valueChanged(qreal value)
{
    for (SvgItemInterface *pItem : qAsConst(mItemList)) {
        pItem->setOpacity(value);
    }
}

void KBlocksAnimFade::endAnimation()
{
    emit animationFinished();
}
