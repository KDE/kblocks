/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2007 Mauricio Piacentini <mauricio@tabuleiro.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
#include "block.h"

#include <QPainter>
#include <QTimeLine>

Block::Block (QGraphicsItem * parent)
  : QGraphicsPixmapItem (parent) , m_opacity(0.0), m_animatingPos(false)
{
}

Block::~Block()
{
}

void Block::setOpacity( qreal opacity )
{
    m_opacity = opacity;
    update();
}

void Block::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    painter->setOpacity( m_opacity );
    QGraphicsPixmapItem::paint( painter, option, widget );
}

bool Block::animatePosSetup ( QPointF initialpos, QPointF finalpos ) { 
  //If we are already animating then keep our initial position
  //TODO: humm... maybe remove us from previous animator so that step is not called twice
  bool alreadyAnimating = m_animatingPos;
  if (!alreadyAnimating) m_animatePosInitial = initialpos; 
  m_animatePosFinal = finalpos ; 
  m_animatingPos = true ;
  return alreadyAnimating;
}

void Block::animatePosStep ( qreal value )
{
    QPointF delta = m_animatePosFinal - m_animatePosInitial;
    delta = delta * value;
    setPos(m_animatePosInitial + delta);
}

void Block::animatePosEnd () { 
  m_animatingPos = false ;
}

//---Piece is basically just a collection of blocks

Piece::Piece ()
{
}

Piece::~Piece()
{
}

void Piece::addItem(Block* ablock)
{
  m_blocks.append(ablock);
}

void Piece::removeItem(Block* ablock)
{
  m_blocks.removeAll(ablock);
}

void Piece::setData(int key, const QVariant & value)
{
  m_data.insert ( key,value);
}

QVariant  Piece::data ( int key )
{
  return m_data.value(key);
}

//#include "block.moc"

