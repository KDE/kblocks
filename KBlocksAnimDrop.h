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
#ifndef KBLOCKSANIMDROP_H
#define KBLOCKSANIMDROP_H

#include <QTimeLine>

#include "KBlocksSvgItem.h"

class KBlocksAnimDrop : public QObject
{
    Q_OBJECT

public:
    KBlocksAnimDrop(const QList<KBlocksSvgItem *> &items, int duration, QTimeLine::Direction direction);
    ~KBlocksAnimDrop();

signals:
    void animationFinished();

private slots:
    void valueChanged(qreal value);
    void endAnimation();

private:
    QTimeLine *mpTimeLine;
    QList<KBlocksSvgItem *> mItemList;
};

#endif
