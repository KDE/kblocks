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
#include "KBlocksView.h"

#include <KDebug>
#include <QResizeEvent>

KBlocksView::KBlocksView(KBlocksScene * scene, QWidget * parent): QGraphicsView(scene, parent)
{
    mpGameScene = scene;
    
    //setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setFrameStyle(QFrame::NoFrame);

    setOptimizationFlags( QGraphicsView::DontClipPainter |
                          QGraphicsView::DontSavePainterState /*|
                          QGraphicsView::DontAdjustForAntialiasing*/ );
    
    setCacheMode(QGraphicsView::CacheBackground);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

KBlocksView::~KBlocksView()
{
}

void KBlocksView::settingsChanged()
{
    mpGameScene->readSettings(size());
    fitInView(mpGameScene->sceneRect(), Qt::KeepAspectRatio);
}

void KBlocksView::focusInEvent(QFocusEvent *)
{
    emit focusEvent(false);
}

void KBlocksView::focusOutEvent(QFocusEvent *)
{
    emit focusEvent(true);
}

void KBlocksView::resizeEvent(QResizeEvent * event)
{
    fitInView(mpGameScene->sceneRect(), Qt::KeepAspectRatio);
    event->accept();
}

