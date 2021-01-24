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
#ifndef KBLOCKSANIMFADE_H
#define KBLOCKSANIMFADE_H

#include <QTimeLine>

class SvgItemInterface;

class KBlocksAnimFade : public QObject
{
    Q_OBJECT

public:
    KBlocksAnimFade(const QList<SvgItemInterface *> &items, int duration, QTimeLine::Direction direction);
    ~KBlocksAnimFade();

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
