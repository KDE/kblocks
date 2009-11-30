#include "KBlocksSvgItem.h"

#include <QPainter>

KBlocksSvgItem::KBlocksSvgItem()
{
    mpGameLayout = 0;
    
    mPosX = 0;
    mPosY = 0;
    
    mType = KBlocksSvgItem_None;
    mColor = -1;
    
    mOpacity = 1;
}

KBlocksSvgItem::~KBlocksSvgItem()
{
}

void KBlocksSvgItem::setLayoutHandler(KBlocksLayout * p)
{
    mpGameLayout = p;
}

void KBlocksSvgItem::setLayoutPos(int posX, int posY)
{
    mPosX = posX;
    mPosY = posY;
}

void KBlocksSvgItem::setLayoutType(int type)
{
    mType = type;
}

bool KBlocksSvgItem::updateSelf()
{
    int tmpColor;
    
    if (mType == KBlocksSvgItem_FieldArea)
    {
        tmpColor = mpGameLayout->getFieldColor(mPosX, mPosY);
    }
    else if (mType == KBlocksSvgItem_PrepareArea)
    {
        tmpColor = mpGameLayout->getPrepareColor(mPosX, mPosY);
    }
    else
    {
        return false;
    }
    
    if (mColor != tmpColor)
    {
        mColor = tmpColor;
        if (mColor == -1)
        {
            setVisible(false);
        }
        else
        {
            setElementId(QString("BLOCK_%1").arg(mColor));
            setVisible(true);
        }
    }
    
    return true;
}

void KBlocksSvgItem::setOpacity(qreal opacity)
{
    mOpacity = opacity;
    update();
}
void KBlocksSvgItem::startPosAnim(QPointF target)
{
    mOriginPos = pos();
    mTargetPos = pos()+ target;
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

void KBlocksSvgItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->setOpacity(mOpacity);
    QGraphicsSvgItem::paint(painter, option, widget);
}

