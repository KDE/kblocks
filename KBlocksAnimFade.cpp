/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Mauricio Piacentini <mauricio@tabuleiro.com>       *
*                      Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksAnimFade.h"

KBlocksAnimFade::KBlocksAnimFade(const QList<KBlocksSvgItem *> &items, int duration, QTimeLine::Direction direction)
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
    foreach (KBlocksSvgItem *pItem, mItemList) {
        pItem->setOpacity(value);
    }
}

void KBlocksAnimFade::endAnimation()
{
    emit animationFinished();
}
