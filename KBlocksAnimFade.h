#ifndef KBLOCKSANIMFADE_H
#define KBLOCKSANIMFADE_H

#include <QTimeLine>

#include "KBlocksSvgItem.h"

class KBlocksAnimFade : public QObject
{
    Q_OBJECT
    
    public:
        KBlocksAnimFade(const QList<KBlocksSvgItem*> & items, int duration, QTimeLine::Direction direction);
        ~KBlocksAnimFade();
        
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
