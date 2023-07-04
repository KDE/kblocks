/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Mauricio Piacentini <mauricio@tabuleiro.com>       *
*                      Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
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

#include "moc_KBlocksSvgItem.cpp"
