/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2009-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
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
    ~KBlocksAnimFade() override;

Q_SIGNALS:
    void animationFinished();

private Q_SLOTS:
    void valueChanged(qreal value);
    void endAnimation();

private:
    QTimeLine *mpTimeLine;
    QList<SvgItemInterface *> mItemList;
};

#endif
