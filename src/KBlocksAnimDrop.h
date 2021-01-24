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
#ifndef KBLOCKSANIMDROP_H
#define KBLOCKSANIMDROP_H

#include <QTimeLine>

class SvgItemInterface;

class KBlocksAnimDrop : public QObject
{
    Q_OBJECT

public:
    KBlocksAnimDrop(const QList<SvgItemInterface *> &items, int duration, QTimeLine::Direction direction);
    ~KBlocksAnimDrop();

signals:
    void animationFinished();

private slots:
    void valueChanged(qreal value);
    void endAnimation();

private:
    QTimeLine *mpTimeLine;
    QList<SvgItemInterface *> mItemList;
};

#endif
