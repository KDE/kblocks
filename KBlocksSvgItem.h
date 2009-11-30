#ifndef KBLOCKSSVGITEM_H
#define KBLOCKSSVGITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSvgItem>

#include "KBlocksLayout.h"

enum KBlocks_SvgItem_Type {
    KBlocksSvgItem_None = -1,
    KBlocksSvgItem_FieldArea = 0,
    KBlocksSvgItem_PrepareArea,
    KBlocksSvgItem_Max_Count,
};

class KBlocksSvgItem : public QGraphicsSvgItem
{
    Q_OBJECT
    
    public:
        KBlocksSvgItem();
        ~KBlocksSvgItem();
        
        void setLayoutHandler(KBlocksLayout * p);
        void setLayoutPos(int posX, int posY);
        void setLayoutType(int type);
        
        bool updateSelf();
        
        void setOpacity(qreal opacity);
        
        void startPosAnim(QPointF target);
        void execPosAnim(qreal step);
        void stopPosAnim();
        
    private:
        void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    private:
        KBlocksLayout* mpGameLayout;
        
        int mPosX;
        int mPosY;
        
        int mType;
        int mColor;
        
        qreal mOpacity;
        
        QPointF mOriginPos;
        QPointF mTargetPos;
};

#endif
