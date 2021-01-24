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
    emit animFinished(KBlocks_Animation_Fade_In);
}

void KBlocksAnimator::endFadeOutAnim()
{
    emit animFinished(KBlocks_Animation_Fade_Out);
}

void KBlocksAnimator::endDropAnim()
{
    emit animFinished(KBlocks_Animation_Drop);
}
