/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include "KBlocksAnimator.h"

#include <KDebug>

KBlocksAnimator::KBlocksAnimator()
{
    mpAnimFade = 0;
    mpAnimDrop = 0;
}

KBlocksAnimator::~KBlocksAnimator()
{
    if (mpAnimDrop)
    {
        delete mpAnimDrop;
        mpAnimDrop = 0;
    }
    if (mpAnimFade)
    {
        delete mpAnimFade;
        mpAnimFade = 0;
    }
}

bool KBlocksAnimator::createFadeAnim(const QList<KBlocksSvgItem*> & items, int duration, QTimeLine::Direction direction)
{
    if (mpAnimFade)
    {
        return false;
    }
    
    mpAnimFade = new KBlocksAnimFade(items, duration, direction);
    
    if (direction == QTimeLine::Forward)
    {
        connect(mpAnimFade, SIGNAL(animationFinished()), this, SLOT(endFadeInAnim()));
    }
    else
    {
        connect(mpAnimFade, SIGNAL(animationFinished()), this, SLOT(endFadeOutAnim()));
    }
    
    return true;
}

bool KBlocksAnimator::deleteFadeAnim()
{
    if (mpAnimFade)
    {
        delete mpAnimFade;
        mpAnimFade = 0;
        return true;
    }
    return false;
}

KBlocksAnimFade* KBlocksAnimator::getFadeAnim()
{
    return mpAnimFade;
}

bool KBlocksAnimator::createDropAnim(const QList<KBlocksSvgItem*> & items, int duration, QTimeLine::Direction direction)
{
    if (mpAnimDrop)
    {
        return false;
    }
    
    mpAnimDrop = new KBlocksAnimDrop(items, duration, direction);
    
    connect(mpAnimDrop, SIGNAL(animationFinished()), this, SLOT(endDropAnim()));
    
    return true;
}

bool KBlocksAnimator::deleteDropAnim()
{
    if (mpAnimDrop)
    {
        delete mpAnimDrop;
        mpAnimDrop = 0;
        return true;
    }
    return false;
}

KBlocksAnimDrop* KBlocksAnimator::getDropAnim()
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
