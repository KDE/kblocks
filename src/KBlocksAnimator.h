/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2009-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#ifndef KBLOCKSANIMATOR_H
#define KBLOCKSANIMATOR_H

#include <QTimeLine>

#include "KBlocksAnimFade.h"
#include "KBlocksAnimDrop.h"

class SvgItemInterface;

enum KBlocks_Animation_Type {
    KBlocks_Animation_None = 0,
    KBlocks_Animation_Fade_In,
    KBlocks_Animation_Fade_Out,
    KBlocks_Animation_Drop,
    KBlocks_Animation_Max_Count
};

class KBlocksAnimator : public QObject
{
    Q_OBJECT

public:
    KBlocksAnimator();
    ~KBlocksAnimator() override;

    bool createFadeAnim(const QList<SvgItemInterface *> &items, int duration, QTimeLine::Direction direction);
    bool deleteFadeAnim();
    KBlocksAnimFade *getFadeAnim();

    bool createDropAnim(const QList<SvgItemInterface *> &items, int duration, QTimeLine::Direction direction);
    bool deleteDropAnim();
    KBlocksAnimDrop *getDropAnim();

private Q_SLOTS:
    void endFadeInAnim();
    void endFadeOutAnim();
    void endDropAnim();

Q_SIGNALS:
    void animFinished(int animType);

protected:
    KBlocksAnimFade *mpAnimFade = nullptr;
    KBlocksAnimDrop *mpAnimDrop = nullptr;
};

#endif
