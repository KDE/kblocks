/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2009-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#include "KBlocksAnimDrop.h"

#include "SvgItemInterface.h"

KBlocksAnimDrop::KBlocksAnimDrop(const QList<SvgItemInterface *> &items, int duration, QTimeLine::Direction direction)
{
    mItemList = items;

    mpTimeLine = new QTimeLine(duration);
    mpTimeLine->setFrameRange(0, 30);
    mpTimeLine->setDirection(direction);

    connect(mpTimeLine, &QTimeLine::valueChanged, this, &KBlocksAnimDrop::valueChanged);
    connect(mpTimeLine, &QTimeLine::finished, this, &KBlocksAnimDrop::endAnimation);

    mpTimeLine->start();
}

KBlocksAnimDrop::~KBlocksAnimDrop()
{
    mpTimeLine->deleteLater();
}

void KBlocksAnimDrop::valueChanged(qreal value)
{
    Q_UNUSED(value);
    for (SvgItemInterface *pItem : std::as_const(mItemList)) {
        pItem->execPosAnim(value);
    }
}

void KBlocksAnimDrop::endAnimation()
{
    Q_EMIT animationFinished();
}

#include "moc_KBlocksAnimDrop.cpp"
