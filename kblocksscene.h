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
#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup>
#include <QTimer>

#include "block.h"
#include "blockanimator.h"

//A list of coordinates (QPoints) defines the blocks that form a piece in a particular rotation
typedef QList<QPoint> PieceRotation;
//And a list of possible rotations for the same basic piece type makes up a PieceSet
typedef QList<PieceRotation> PieceSet;

enum KBlocksGameState { Game_Starting=0, 
  Game_Active,
  Game_Paused,
  Game_Suspended, //not by user
  Game_Finished };

enum KBlocksScoreEvent { Score_Blocks=0,
  Score_Lines,
  Score_Level };
  
enum KBlocksRotationDirection { Rotate_Clockwise=0,
    Rotate_CounterClockwise };

class KGamePopupItem;
class KBlocksGraphics;
class KBlocksSound;

class KBlocksScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit KBlocksScene();
    virtual ~KBlocksScene();
    
  signals:
    void scoreChanged(int,int,int);
    void isHighscore(int,int);
    
  public slots:
    void pauseGame(bool pause, bool fromUI);
    void startGame();
    void readSettings(const QSize & viewSize);
    void setSoundsEnabled(bool enabled);
    void viewScaled(const QSize& newsize);
    void attemptMove(const QPoint& delta);
    void attemptPushDown(const QPoint& delta);
    void attemptRotation(KBlocksRotationDirection direction);

private:
    KBlocksGraphics *grafx;
    KBlocksSound *snd;
    QList<QObject *> animators;
    QList<Piece *> activePieces;
    QList<Block *> frozenBlocks;
    QList<PieceSet> pieceTypes;
    Piece * nextPiece;
    
    QTimer stepTimer;
    QTimer releaseTimer;
    
    //QGraphicsPixmapItem * bg;
    QGraphicsSvgItem * playArea;
    //QGraphicsTextItem * scoreArea;
    //QGraphicsTextItem * levelArea;
    
    KGamePopupItem *messageItem;
    
    int gameState;
    int previousGameState;
    int currentLevel;
    int currentPoints;
    int currentRemovedLines;
    int inLockPosition;
    int updateInterval;
    bool firstBlock;
    
    void cleanAll();
    void levelUp();
    void updateDimensions();
    void initPieceTypes();
    bool canMove(Piece *piece, const QPoint& delta);
    bool canRotate(Piece * piece, KBlocksRotationDirection direction);
    bool isTrappedAtTop(Piece * piece);
    void movePiece(Piece * piece, const QPoint& delta);
    void moveBlock(Block * block, QPoint delta);
    void rotatePiece(Piece * piece, KBlocksRotationDirection direction);
    void freezePiece(Piece * piece);
    int searchForCompleteLines();
    void removeLine(int liney);
    void prepareNewPiece();
    int coordToIndex(const QPoint& coord);
    QPoint indexToCoord(int idx);
    QPointF coordToPoint(const QPoint& coord);
    QPointF nextPieceCoordToPoint(const QPoint& coord);
    QPointF pieceCenterPoint(Piece * piece);
    void centerPiecePreview(Piece * piece);
    void showMessage( const QString& message, int ms );
    void addToScore(KBlocksScoreEvent type, int count);
    
private slots:
    void step();
    void animationFinished(QObject * animation);
    void releasePiece();
    void greetPlayer();
    void checkHighscore();
    
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *keyEvent);
    void drawBackground ( QPainter * painter, const QRectF & rect );
};

#endif //SCENE
