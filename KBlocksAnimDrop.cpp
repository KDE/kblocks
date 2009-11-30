#include "KBlocksAnimDrop.h"

KBlocksAnimDrop::KBlocksAnimDrop(const QList<KBlocksSvgItem*> & items, int duration, QTimeLine::Direction direction)
{
    mItemList = items;
    
    mpTimeLine = new QTimeLine(duration);
    mpTimeLine->setFrameRange( 0, 30 );
    mpTimeLine->setDirection(direction);
    
    connect(mpTimeLine, SIGNAL(valueChanged(qreal)), SLOT(valueChanged(qreal)));
    connect(mpTimeLine, SIGNAL(finished()), SLOT(endAnimation()));
    
    mpTimeLine->start();
}

KBlocksAnimDrop::~KBlocksAnimDrop()
{
    delete mpTimeLine;
}

void KBlocksAnimDrop::valueChanged(qreal value)
{
    Q_UNUSED(value);
    foreach(KBlocksSvgItem * pItem, mItemList)
    {
        pItem->execPosAnim(value);
    }
}

void KBlocksAnimDrop::endAnimation()
{
    emit animationFinished();
}
