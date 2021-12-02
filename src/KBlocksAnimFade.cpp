/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2009-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
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
    for (SvgItemInterface *pItem : std::as_const(mItemList)) {
        pItem->setOpacity(value);
    }
}

void KBlocksAnimFade::endAnimation()
{
    Q_EMIT animationFinished();
}
