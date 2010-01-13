/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Mauricio Piacentini <mauricio@tabuleiro.com>       *
*                      Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSVIEW_H
#define KBLOCKSVIEW_H

#include <QGraphicsView>

#include "KBlocksScene.h"

class KBlocksView : public QGraphicsView
{
    Q_OBJECT

    public:
        KBlocksView(KBlocksScene * scene, QWidget * parent = 0);
        ~KBlocksView();
        
    signals:
        void focusEvent(bool flag);
        
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
