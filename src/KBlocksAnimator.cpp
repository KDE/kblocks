/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2009-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#include "KBlocksAnimator.h"

#include "SvgItemInterface.h"

KBlocksAnimator::KBlocksAnimator()
{
}

KBlocksAnimator::~KBlocksAnimator()
{
    if (mpAnimDrop) {
        delete mpAnimDrop;
        mpAnimDrop = nullptr;
    }
    if (mpAnimFade) {
        delete mpAnimFade;
        mpAnimFade = nullptr;
    }
}

bool KBlocksAnimator::createFadeAnim(const QList<SvgItemInterface *> &items, int duration, QTimeLine::Direction direction)
{
    if (mpAnimFade) {
        return false;
    }

    mpAnimFade = new KBlocksAnimFade(items, duration, direction);

    if (direction == QTimeLine::Forward) {
        connect(mpAnimFade, &KBlocksAnimFade::animationFinished, this, &KBlocksAnimator::endFadeInAnim);
    } else {
        connect(mpAnimFade, &KBlocksAnimFade::animationFinished, this, &KBlocksAnimator::endFadeOutAnim);
    }

    return true;
}

bool KBlocksAnimator::deleteFadeAnim()
{
    if (mpAnimFade) {
        delete mpAnimFade;
        mpAnimFade = nullptr;
        return true;
    }
    return false;
}

KBlocksAnimFade *KBlocksAnimator::getFadeAnim()
{
    return mpAnimFade;
}

bool KBlocksAnimator::createDropAnim(const QList<SvgItemInterface *> &items, int duration, QTimeLine::Direction direction)
{
    if (mpAnimDrop) {
        return false;
    }

    mpAnimDrop = new KBlocksAnimDrop(items, duration, direction);

    connect(mpAnimDrop, &KBlocksAnimDrop::animationFinished, this, &KBlocksAnimator::endDropAnim);

    return true;
}

bool KBlocksAnimator::deleteDropAnim()
{
    if (mpAnimDrop) {
        delete mpAnimDrop;
        mpAnimDrop = nullptr;
        return true;
    }
    return false;
}

KBlocksAnimDrop *KBlocksAnimator::getDropAnim()
{
    return mpAnimDrop;
}

void KBlocksAnimator::endFadeInAnim()
{
    Q_EMIT animFinished(KBlocks_Animation_Fade_In);
}

void KBlocksAnimator::endFadeOutAnim()
{
    Q_EMIT animFinished(KBlocks_Animation_Fade_Out);
}

void KBlocksAnimator::endDropAnim()
{
    Q_EMIT animFinished(KBlocks_Animation_Drop);
}
