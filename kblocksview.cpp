/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2007 Mauricio Piacentini <mauricio@tabuleiro.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
#include "kblocksview.h"
#include "kblocksscene.h"

KBlocksView::KBlocksView (QWidget * parent): QGraphicsView(parent)
{
  m_scene = new KBlocksScene();
  setScene(m_scene);
  //setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setFrameStyle(QFrame::NoFrame);
  
  //setMinimumSize ( 585, 660 );
  setMaximumSize ( 585, 660 );

  setOptimizationFlags( QGraphicsView::DontClipPainter |
      QGraphicsView::DontSavePainterState |
      QGraphicsView::DontAdjustForAntialiasing );
  setCacheMode(QGraphicsView::CacheBackground);
  //setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
  show();
}

KBlocksView::~KBlocksView ()
{
  delete m_scene;
}

void KBlocksView::newGame()
{
  m_scene->startGame();
}

void KBlocksView::pauseGame()
{
  m_scene->pauseGame();
}

void KBlocksView::resizeEvent(QResizeEvent* event) {
  fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
  QGraphicsView::resizeEvent(event);
}

#include "kblocksview.moc"
