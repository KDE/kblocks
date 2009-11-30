#include "KBlocksAnimFade.h"

#include <KDebug>

KBlocksAnimFade::KBlocksAnimFade(const QList<KBlocksSvgItem*> & items, int duration, QTimeLine::Direction direction)
{
    mItemList = items;
    
    mpTimeLine = new QTimeLine(duration);
    mpTimeLine->setFrameRange( 0, 30 );
    mpTimeLine->setDirection(direction);
    
    connect(mpTimeLine, SIGNAL(valueChanged(qreal)), SLOT(valueChanged(qreal)));
    connect(mpTimeLine, SIGNAL(finished()), SLOT(endAnimation()));
    
    mpTimeLine->start();
}

KBlocksAnimFade::~KBlocksAnimFade()
{
    delete mpTimeLine;
}

void KBlocksAnimFade::valueChanged(qreal value)
{
    foreach(KBlocksSvgItem * pItem, mItemList)
    {
        pItem->setOpacity(value);
    }
}

void KBlocksAnimFade::endAnimation()
{
    emit animationFinished();
}
