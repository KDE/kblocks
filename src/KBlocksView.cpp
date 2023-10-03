/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
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

void KBlocksView::loadTheme(const KGameTheme *theme)
{
    SceneInterface* s = dynamic_cast<SceneInterface*>(scene());
    if (s) {
        s->loadTheme(theme);
    }
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

void KBlocksView::settingsChanged()
{
    SceneInterface* s = dynamic_cast<SceneInterface*>(scene());
    if (s) {
        s->readSettings();
    }
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

void KBlocksView::focusInEvent(QFocusEvent *)
{
    Q_EMIT focusEvent(false);
}

void KBlocksView::focusOutEvent(QFocusEvent *)
{
    Q_EMIT focusEvent(true);
}

void KBlocksView::resizeEvent(QResizeEvent *event)
{
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    event->accept();
}

#include "moc_KBlocksView.cpp"
