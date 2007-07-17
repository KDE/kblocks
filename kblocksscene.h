/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2007 Mauricio Piacentini <mauricio@tabuleiro.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QtSvg>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup>
#include <QTimer>

#include "block.h"
#include "blockanimator.h"

//A list of coordinates (QPoints) defines the blocks that form a piece in a particular rotation
typedef QList<QPoint> PieceRotation;
//And a list of possible rotations for the same basic piece type makes up a PieceSet
typedef QList<PieceRotation> PieceSet;

class KGamePopupItem;
class KBlocksGraphics;

class KBlocksScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit KBlocksScene();
    virtual ~KBlocksScene();
    
  public slots:
    void pauseGame();
    void startGame();

private:
    KBlocksGraphics *grafx;
    QList<QObject *> animators;
    QList<Piece *> activePieces;
    QList<Block *> frozenBlocks;
    QList<PieceSet> pieceTypes;
    Piece * nextPiece;
    
    QTimer stepTimer;
    
    //QGraphicsPixmapItem * bg;
    QGraphicsPixmapItem * playArea;
    
    KGamePopupItem *messageItem;
    
    bool m_paused;
    
    void cleanAll();
    void initPieceTypes();
    bool canMove(Piece *piece, const QPoint& delta);
    bool canRotate(Piece * piece);
    bool isTrappedAtTop(Piece * piece);
    void movePiece(Piece * piece, const QPoint& delta);
    void moveBlock(Block * block, QPoint delta);
    void rotatePiece(Piece * piece);
    void attemptMove(const QPoint& delta);
    void attemptRotation();
    void freezePiece(Piece * piece);
    void searchForCompleteLines();
    void removeLine(int liney);
    void prepareNewPiece();
    int coordToIndex(const QPoint& coord);
    QPoint indexToCoord(int idx);
    QPointF coordToPoint(const QPoint& coord);
    void showMessage( const QString& message, int ms );
    
private slots:
    void step();
    void animationFinished(QObject * animation);
    void releasePiece();
    void greetPlayer();
    
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *keyEvent);
    void drawBackground ( QPainter * painter, const QRectF & rect );
    void keyPressEvent(QKeyEvent *event);
};

#endif //SCENE
