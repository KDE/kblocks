/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2007 Mauricio Piacentini <mauricio@tabuleiro.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
#include "blockanimator.h"
#include "block.h"

BlockAnimator::BlockAnimator(const QList<Block *> & blocks, int duration, QTimeLine::Direction direction, bool deleteWhenDone)
{
  m_blocks = blocks;
  m_timeLine = new QTimeLine(duration);
  m_timeLine->setFrameRange( 0, 30 );
  m_timeLine->setDirection(direction);
  m_deleteBlocksWhenDone = deleteWhenDone;
  connect(m_timeLine, SIGNAL(valueChanged(qreal)), SLOT(valueChanged(qreal)) );
  connect(m_timeLine, SIGNAL(finished()), SLOT(animationFinished()) );
  m_timeLine->start();
}

BlockAnimator::~BlockAnimator()
{
  delete m_timeLine;
}

void BlockAnimator::removeBlock(Block * block)
{
  m_blocks.removeAll(block);
}

void BlockAnimator::valueChanged( qreal value )
{
  Q_UNUSED(value);
}

void BlockAnimator::animationFinished( )
{
  if (m_deleteBlocksWhenDone) {
    foreach (Block* block, m_blocks) {
      block->scene()->removeItem(block);
      delete block;
    }
  }
  emit finished(this);
}

///---------------

void FadeAnimator::valueChanged( qreal value )
{
    foreach (Block* block, m_blocks) {
      block->setOpacity(value);
    }
}

///--------------

void DropAnimator::valueChanged( qreal value )
{
  foreach (Block* block, m_blocks) {
    block->animatePosStep(value);
  }
}
void DropAnimator::animationFinished( )
{
  foreach (Block* block, m_blocks) {
    block->animatePosEnd();
  }
  emit finished(this);
}
