/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2010-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/
#include <QResizeEvent>

#include "KBlocksView.h"
#include "SceneInterface.h"

KBlocksView::KBlocksView(SceneInterface *scene, QWidget *parent): QGraphicsView(scene, parent)
{
    //setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameStyle(QFrame::NoFrame);

    setOptimizationFlags(
                         QGraphicsView::DontSavePainterState /*|
                          QGraphicsView::DontAdjustForAntialiasing*/);

    setCacheMode(QGraphicsView::CacheBackground);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

KBlocksView::~KBlocksView()
{
}

void KBlocksView::settingsChanged()
{
    SceneInterface* s = dynamic_cast<SceneInterface*>(scene());
    if (s) {
        s->readSettings(size());
    }
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

void KBlocksView::focusInEvent(QFocusEvent *)
{
    emit focusEvent(false);
}

void KBlocksView::focusOutEvent(QFocusEvent *)
{
    emit focusEvent(true);
}

void KBlocksView::resizeEvent(QResizeEvent *event)
{
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    event->accept();
}

