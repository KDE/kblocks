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
#include <KDebug>

KBlocksView::KBlocksView (QWidget * parent): QGraphicsView(parent)
{
  m_scene = new KBlocksScene();
  setScene(m_scene);
  //setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setFrameStyle(QFrame::NoFrame);
  setMinimumSize ( 120, 120 );

  setOptimizationFlags( QGraphicsView::DontClipPainter |
      QGraphicsView::DontSavePainterState /*|
      QGraphicsView::DontAdjustForAntialiasing*/ );
  setCacheMode(QGraphicsView::CacheBackground);
  setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
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

void KBlocksView::pauseGame(bool pressed)
{
  m_scene->pauseGame(pressed, true);
}

void KBlocksView::settingsChanged()
{
  m_scene->readSettings(size());
  fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void KBlocksView::rotateCW()
{
  m_scene->attemptRotation(Rotate_Clockwise);
}

void KBlocksView::rotateCCW()
{
  m_scene->attemptRotation(Rotate_CounterClockwise);
}

void KBlocksView::moveLeft()
{
  m_scene->attemptMove(QPoint(-1,0));
}

void KBlocksView::moveRight()
{
  m_scene->attemptMove(QPoint(1,0));
}

void KBlocksView::moveDown()
{
  m_scene->attemptMove(QPoint(0,1));
}

void KBlocksView::focusInEvent ( QFocusEvent * event )
{
  //Recover from suspended state, sinalize it was not initiated by UI
  m_scene->pauseGame(false,false);
}
void KBlocksView::focusOutEvent ( QFocusEvent * event )
{
  //Force suspended state, sinalize it was not initiated by UI
  m_scene->pauseGame(true, false);
}

void KBlocksView::resizeEvent(QResizeEvent* event) {
  fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
  //if (event->spontaneous()) return;
  //m_scene->viewScaled(event->size());
  
  //QGraphicsView::resizeEvent(event);
  event->accept();
}

#include "kblocksview.moc"
