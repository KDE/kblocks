#ifndef KBLOCKSANIMDROP_H
#define KBLOCKSANIMDROP_H

#include <QTimeLine>

#include "KBlocksSvgItem.h"

class KBlocksAnimDrop : public QObject
{
    Q_OBJECT
    
    public:
        KBlocksAnimDrop(const QList<KBlocksSvgItem*> & items, int duration, QTimeLine::Direction direction);
        ~KBlocksAnimDrop();
        
    signals:
        void animationFinished();
        
    private slots:
        void valueChanged(qreal value);
        void endAnimation();
        
    private:
        QTimeLine* mpTimeLine;
        QList<KBlocksSvgItem*> mItemList;
};

#endif