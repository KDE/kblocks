#ifndef KBLOCKSVIEW_H
#define KBLOCKSVIEW_H

#include <QGraphicsView>

#include "KBlocksScene.h"

class KBlocksView : public QGraphicsView
{
    Q_OBJECT

    public:
        KBlocksView(QWidget * parent = 0);
        ~KBlocksView();
        
        void setGameScene(KBlocksScene * p);
        
    public slots:
        void settingsChanged();
    
    protected:
        void focusInEvent(QFocusEvent * event);
        void focusOutEvent(QFocusEvent * event);
        void resizeEvent(QResizeEvent * event);

    protected:
        KBlocksScene* mpGameScene;
};

#endif
