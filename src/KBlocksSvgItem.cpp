/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Mauricio Piacentini <mauricio@tabuleiro.com>       *
*                      Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksSvgItem.h"


KBlocksSvgItem::KBlocksSvgItem(KBlocksLayout *p, int type, int posX, int posY)
{
    mpGameLayout = p;

    mPosX = posX;
    mPosY = posY;

    mType = type;
    mColor = -1;

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

KBlocksSvgItem::~KBlocksSvgItem()
{
}

void KBlocksSvgItem::setLayoutPos(int posX, int posY)
{
    mPosX = posX;
    mPosY = posY;
}

bool KBlocksSvgItem::updateSelf()
{
    int tmpColor;

    if (mType == KBlocksSvgItem_FieldArea) {
        tmpColor = mpGameLayout->getFieldColor(mPosX, mPosY);
    } else if (mType == KBlocksSvgItem_PrepareArea) {
        tmpColor = mpGameLayout->getPrepareColor(mPosX, mPosY);
    } else {
        return false;
    }

    if (mColor != tmpColor) {
        mColor = tmpColor;
        if (mColor == -1) {
            setVisible(false);
        } else {
            setElementId(QStringLiteral("BLOCK_%1").arg(mColor));
            setVisible(true);
        }
    }

    return true;
}

void KBlocksSvgItem::clearCache()
{
    setCacheMode(NoCache);
    setCacheMode(DeviceCoordinateCache);
}


void KBlocksSvgItem::startOpAnim()
{
    setElementId(QStringLiteral("BLOCK_OUT_%1").arg(mColor));
}

void KBlocksSvgItem::stopOpAnim()
{
    setElementId(QStringLiteral("BLOCK_%1").arg(mColor));
}

void KBlocksSvgItem::startPosAnim(QPointF target)
{
    mOriginPos = pos();
    mTargetPos = pos() + target;
}

void KBlocksSvgItem::execPosAnim(qreal step)
{
    QPointF delta = mTargetPos - mOriginPos;
    delta = delta * step;
    setPos(mOriginPos + delta);
}

void KBlocksSvgItem::stopPosAnim()
{
    setPos(mOriginPos);
    mTargetPos = mOriginPos;
}
