/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2007 Mauricio Piacentini <mauricio@tabuleiro.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
#ifndef BLOCKANIMATOR_H
#define BLOCKANIMATOR_H

#include <QGraphicsView>
#include <QTimeLine>

class Block;

class BlockAnimator : public QObject
{
  Q_OBJECT
  public:
    explicit BlockAnimator(const QList<Block *> & blocks, int duration, QTimeLine::Direction direction, bool deleteWhenDone);
    virtual ~BlockAnimator();
  public slots:
    virtual void removeBlock(Block * block);
  signals:
    virtual void finished(QObject * animation);
  private slots:
    virtual void valueChanged( qreal value );
    virtual void animationFinished();
  protected:
    QTimeLine *m_timeLine;
    QList<Block *> m_blocks;
    bool  m_deleteBlocksWhenDone;
};

class FadeAnimator : public BlockAnimator
{
  Q_OBJECT
  public:
    explicit FadeAnimator(const QList<Block *> & blocks, int duration, QTimeLine::Direction direction, bool deleteWhenDone):BlockAnimator(blocks, duration, direction, deleteWhenDone){}
    
  private slots:
    void valueChanged( qreal value );
};

class DropAnimator : public BlockAnimator
{
  Q_OBJECT
  public:
    explicit DropAnimator(const QList<Block *> & blocks, int duration, QTimeLine::Direction direction, bool deleteWhenDone):BlockAnimator(blocks, duration, direction, deleteWhenDone){}

  private slots:
    void valueChanged( qreal value );
    void animationFinished();

};
#endif
