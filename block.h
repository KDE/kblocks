/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2007 Mauricio Piacentini <mauricio@tabuleiro.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
#ifndef BLOCK_H
#define BLOCK_H

#include <QtSvg>
#include <QGraphicsPixmapItem>

class Block : public QObject, public QGraphicsPixmapItem
{
  Q_OBJECT
  public:
    explicit Block (QGraphicsItem * parent);
    virtual ~Block();
    
  public slots:
    void setOpacity( qreal opacity );
    bool animatePosSetup ( QPointF initialpos, QPointF finalpos );
    void animatePosStep ( qreal value );
    void animatePosEnd ();
  private:
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    qreal m_opacity;
    QPointF m_animatePosInitial;
    QPointF m_animatePosFinal;
    bool    m_animatingPos;
};

class Piece
{
  public:
    explicit Piece ();
    virtual ~Piece();
  
    void addItem(Block* ablock);
    void removeItem(Block* ablock);
    void setData(int key, const QVariant & value);
    QVariant data ( int key );
    QList<Block *> children () const { return m_blocks; }
    
    QList<Block *> m_blocks;
    QHash<int, QVariant> m_data;
};

enum PrivateItemData { Block_Coord = 0, Block_OffsetInPiece, Block_Color, Piece_Set, Piece_Rotation };

#endif
