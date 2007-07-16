/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2007 Mauricio Piacentini <mauricio@tabuleiro.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
#include "kblocksscene.h"
#include <QtDebug>
#include <KStandardDirs>
#include <KGamePopupItem>

//Fixed values for now, will be read from Theme SVG soon
const int BLOCK_SIZE = 30;
const int UPDATE_INTERVAL = 300;
const int WIDTH = 585;
const int HEIGHT = 660;
const int FIELD_OFFSET_HEIGHT = 14;
const int FIELD_OFFSET_WIDTH = 49;

const int FIELD_HEIGHT = 20;
const int FIELD_WIDTH  = 10;

KBlocksScene::KBlocksScene() : m_paused(false)
{
    initPieceTypes();
    nextPiece = new Piece();
    renderer = new QSvgRenderer(KStandardDirs::locate("appdata", "themes/default_theme.svg"));
    setSceneRect(0, 0, WIDTH, HEIGHT);
    //fieldOffset.setX((WIDTH-(BLOCK_SIZE*FIELD_WIDTH))/2);
    //fieldOffset.setY((HEIGHT-(BLOCK_SIZE*FIELD_HEIGHT))/2);
    fieldOffset.setX(FIELD_OFFSET_WIDTH);
    fieldOffset.setY(FIELD_OFFSET_HEIGHT);

    //playArea is our first item, non parented. We add it explicitally.
    playArea = new QGraphicsPixmapItem();
    //playArea->setSharedRenderer(renderer);
    //playArea->setElementId("FIELD_AREA");
    playArea->setPixmap(renderElement(WIDTH, HEIGHT, QString("FIELD_AREA")));
    addItem(playArea);
    //playarea->setPos(fieldOffset);
/*
    for (int i = 0; i <= FIELD_HEIGHT; ++i) {
      QGraphicsPixmapItem * wall = new QGraphicsPixmapItem(bg);
      wall->setPixmap(wallpix);
      wall->setPos(coordToPoint(QPoint(FIELD_WIDTH,i)));
      //addItem(wall);
    }
    for (int i = 0; i < FIELD_WIDTH; ++i) {
      QGraphicsPixmapItem * wall = new QGraphicsPixmapItem(bg);
      wall->setPixmap(wallpix);
      wall->setPos(coordToPoint(QPoint(i,FIELD_HEIGHT)));
      //addItem(wall);
    }*/
    
    //Our Message Item, hidden by default
    messageItem = new KGamePopupItem();
    messageItem->setMessageOpacity(0.9);
    addItem(messageItem);

    setItemIndexMethod(NoIndex);
    stepTimer.setInterval(UPDATE_INTERVAL);
    connect(&stepTimer, SIGNAL(timeout()), SLOT(step()));

    startGame();
}

KBlocksScene::~KBlocksScene()
{
  cleanAll();
  delete nextPiece;
  //delete bg;
  delete renderer;
}

void KBlocksScene::drawBackground ( QPainter * painter, const QRectF & rect )
{
  renderer->render(painter, QString("BACKGROUND"), rect);
}

void KBlocksScene::step()
{
    foreach (Piece* piece, activePieces) {
      //check if we can move down
      if (canMove(piece, QPoint(0,1))) {
        movePiece(piece, QPoint(0,1));
      } else {
        //Has it ended?
        if (isTrappedAtTop(piece)) {
          QString end("Game Over"); 
          showMessage( end, 4000 );
          stepTimer.stop();
        } else {
          //we hit something, stop and detach
          freezePiece(piece);
          releasePiece();
        }
      }
    }
}

void KBlocksScene::cleanAll()
{
  foreach (Piece* piece, activePieces) {
    foreach (Block *block, piece->children()) {
      piece->removeItem(block);
      frozenBlocks.append(block);
    }
    delete piece;
  }
  foreach (Block *block, nextPiece->children()) {
    nextPiece->removeItem(block);
    frozenBlocks.append(block);
  }
  foreach (QObject* animator, animators) {
    delete animator;
  }
  foreach (Block* block, frozenBlocks) {
    removeItem(block);
    delete block;
  }
  activePieces.clear();
  animators.clear();
  frozenBlocks.clear();
}

void KBlocksScene::startGame()
{
  stepTimer.stop();
  cleanAll();
  stepTimer.start();
  prepareNewPiece();
    //Fire the first piece in two second
  QTimer::singleShot(2000, this, SLOT(releasePiece())); 
  QTimer::singleShot(500, this, SLOT(greetPlayer())); 
}

void KBlocksScene::greetPlayer()
{
   QString start("Game started"); 
   showMessage( start, 2000 ); 
}

void KBlocksScene::pauseGame()
{
  if (m_paused) {
    stepTimer.start();
    QString end("Resuming Game"); 
    showMessage( end, 2000 );
  } else {
    QString end("Game Paused"); 
    showMessage( end, 2000 );
    stepTimer.stop();
  }
  m_paused = !m_paused;
}

void KBlocksScene::attemptMove(const QPoint& delta)
{
  foreach (Piece* piece, activePieces) {
      //check if we can move
    if (canMove(piece, delta)) {
      movePiece(piece, delta);
    } 
  }
}

void KBlocksScene::attemptRotation()
{
  foreach (Piece* piece, activePieces) {
      //check if we can rotate
    if (canRotate(piece)) {
      rotatePiece(piece);
    } 
  }
}

void KBlocksScene::prepareNewPiece()
{
  //Chose one of the available piece types
  int availablepiecetypes = pieceTypes.size();
  int chosenset = rand()%availablepiecetypes; 
  //qDebug() << chosenset;
  PieceSet chosenpieceset = pieceTypes.at(chosenset);
  //From the chosen piece set we now pick an initial orientation
  int availableorientations = chosenpieceset.size();
  int chosenorientation = rand()%availableorientations; 
  PieceRotation chosenpiecerotation = chosenpieceset.at(chosenorientation);

  //Use the piece blueprint to construct our blocks
  for (int i = 0; i < chosenpiecerotation.size(); ++i) {
    Block *block = new Block(playArea);
    //block->setSharedRenderer(renderer);
    //block->setElementId(QString("BLOCK_%1").arg(chosenset));
    block->setPixmap(renderElement(BLOCK_SIZE, BLOCK_SIZE, QString("BLOCK_%1").arg(chosenset)));
    block->setData(Block_OffsetInPiece, chosenpiecerotation.at(i));
    block->setData(Block_Color, chosenset);
    QPoint point = chosenpiecerotation.at(i)+QPoint(14,2);
    block->setData(Block_Coord, point);
    block->setPos(coordToPoint(point));
      //and append them to temporary collection
    nextPiece->addItem(block);
  }
  //Store blueprint data needed to recreate/rotate the piece
  nextPiece->setData(Piece_Set, chosenset);
  nextPiece->setData(Piece_Rotation, chosenorientation);
  
  FadeAnimator * fadeInAnim = new FadeAnimator(nextPiece->children(), 200, QTimeLine::Forward, false);
  animators << fadeInAnim;
  connect(fadeInAnim, SIGNAL(finished(QObject *)), SLOT(animationFinished(QObject *)) );
}

void KBlocksScene::releasePiece()
{
  //Temp storage for our block graphic items
  Piece* thispiece = new Piece();

  //Now recreate from blueprint stored in nextPiece
  int setidx = nextPiece->data(Piece_Set).toInt();
  int rotationidx = nextPiece->data(Piece_Rotation).toInt();
  PieceSet chosenset = pieceTypes.at(setidx);
  PieceRotation chosenpiecerotation = chosenset.at(rotationidx);

  //Use the piece blueprint to construct our blocks
  for (int i = 0; i < chosenpiecerotation.size(); ++i) {
      Block *block = new Block(playArea);
      //block->setSharedRenderer(renderer);
      //block->setElementId(QString("BLOCK_%1").arg(setidx));
      block->setPixmap(renderElement(BLOCK_SIZE, BLOCK_SIZE, QString("BLOCK_%1").arg(setidx)));

      block->setData(Block_OffsetInPiece, chosenpiecerotation.at(i));
      block->setData(Block_Color, setidx);
      QPoint point = chosenpiecerotation.at(i)+QPoint(FIELD_WIDTH/2,0);
      block->setData(Block_Coord, point);
      block->setPos(coordToPoint(point));
      //and append them to temporary collection
      //blocks.append(block);
      //addItem(block);
      thispiece->addItem(block);
  }

  //Store blueprint data needed to recreate/rotate the piece
  thispiece->setData(Piece_Set, setidx);
  thispiece->setData(Piece_Rotation, rotationidx);
  activePieces.append(thispiece);
  
  FadeAnimator * fadeInAnim = new FadeAnimator(thispiece->children(), 200, QTimeLine::Forward, false);
  animators << fadeInAnim;
  connect(fadeInAnim, SIGNAL(finished(QObject *)), SLOT(animationFinished(QObject *)) );
  
  //FadeOut animator also removes blocks from scene and deletes them when it is done :)
  FadeAnimator * fadeOutAnim = new FadeAnimator(nextPiece->children(), 200, QTimeLine::Backward, true);
  animators << fadeOutAnim;
  connect(fadeOutAnim, SIGNAL(finished(QObject *)), SLOT(animationFinished(QObject *)) );
  foreach(Block * block, nextPiece->children()){
    nextPiece->removeItem(block);
  }
  prepareNewPiece();
}

bool KBlocksScene::canMove(Piece * piece, const QPoint& delta)
{
  foreach (QGraphicsItem *pieceblock, piece->children()) {
    QPoint piececoord = pieceblock->data(Block_Coord).toPoint();
    piececoord = piececoord+delta;
    
    //Did we hit the floor?
    if (piececoord.y() >= FIELD_HEIGHT ) return false;
    //Or maybe the edges?
    if ((piececoord.x() >= FIELD_WIDTH)||(piececoord.x() < 0)) return false;
    
    //Check only against the frozenBlocks
    foreach (QGraphicsItem *block, frozenBlocks) {
      QPoint checkcoord = block->data(Block_Coord).toPoint();
      if (checkcoord==piececoord) {
        return false; 
      }
    }
  }
  return true;
}

bool KBlocksScene::isTrappedAtTop(Piece * piece)
{
  foreach (QGraphicsItem *pieceblock, piece->children()) {
    QPoint piececoord = pieceblock->data(Block_Coord).toPoint();
    if (piececoord.y() <= 0 ) return true;
  }
  return false;
}

bool KBlocksScene::canRotate(Piece * piece)
{
  //Retrieve pieceSet and orientation
  int setidx = piece->data(Piece_Set).toInt();
  int rotationidx = piece->data(Piece_Rotation).toInt();
  PieceSet pieceset = pieceTypes.at(setidx);
  PieceRotation curpiecerotation = pieceset.at(rotationidx);
  //get here current delta
  int nextrotationidx = rotationidx+1;
  if (nextrotationidx >= pieceset.count()) {
    //wrap up next rotation
    nextrotationidx = 0;
  }
  PieceRotation testrotation = pieceset.at(nextrotationidx);
  //now find out the current offset using current piece orientation
  //notice that chidren() returns blocks in no particular order, so all have the originalOffset saved
  QGraphicsItem * ablock = piece->children().at(0);
  QPoint delta = (ablock->data(Block_Coord).toPoint())-(ablock->data(Block_OffsetInPiece).toPoint());

  foreach (Block *pieceblock, piece->children()) {
    //actually the delta is the same for all blocks, just test one at a time
    Q_UNUSED(pieceblock);
    QPoint piececoord = testrotation.takeFirst();
    piececoord = piececoord+delta;
    
    //Did we hit the floor?
    if (piececoord.y() >= FIELD_HEIGHT ) return false;
    //Or maybe the edges?
    if ((piececoord.x() >= FIELD_WIDTH)||(piececoord.x() < 0)) return false;
    
    //Check only against the frozenBlocks
    foreach (QGraphicsItem *block, frozenBlocks) {
      QPoint checkcoord = block->data(Block_Coord).toPoint();
      if (checkcoord==piececoord) {
        return false;
      }
    }
  }
  return true;
}

void KBlocksScene::movePiece(Piece * piece, const QPoint& delta)
{
  foreach (Block *pieceblock, piece->children()) {
    moveBlock(pieceblock, delta);
  }
}

void KBlocksScene::moveBlock(Block * block, QPoint delta)
{
  QPoint coord = block->data(Block_Coord).toPoint();
  coord = coord + delta;
  block->setData(Block_Coord, coord);
  block->setPos(coordToPoint(coord));
}

void KBlocksScene::rotatePiece(Piece * piece)
{
  //Retrieve pieceSet and orientation
  int setidx = piece->data(Piece_Set).toInt();
  int rotationidx = piece->data(Piece_Rotation).toInt();
  PieceSet pieceset = pieceTypes.at(setidx);
  PieceRotation curpiecerotation = pieceset.at(rotationidx);
  //get here current delta
  int nextrotationidx = rotationidx+1;
  if (nextrotationidx >= pieceset.count()) {
    //wrap up next rotation
    nextrotationidx = 0;
  }
  PieceRotation nextrotation = pieceset.at(nextrotationidx);
  piece->setData(Piece_Rotation, nextrotationidx);
  //now find out the current offset using current piece orientation
  //notice that chidren() returns blocks in no particular order, so all have the originalOffset saved
  QGraphicsItem * ablock = piece->children().at(0);
  QPoint delta = (ablock->data(Block_Coord).toPoint())-(ablock->data(Block_OffsetInPiece).toPoint());
  //sanity check: test if testrotation.count() = piece->children().count() maybe?

  foreach (Block *pieceblock, piece->children()) {
    QPoint blockcoord = nextrotation.takeFirst();
    pieceblock->setData(Block_OffsetInPiece, blockcoord);
    blockcoord = blockcoord+delta;
    pieceblock->setData(Block_Coord, blockcoord);
    pieceblock->setPos(coordToPoint(blockcoord));
  }
}

void KBlocksScene::freezePiece(Piece * piece)
{
  foreach (Block *block, piece->children()) {
    piece->removeItem(block);
    //frozenBlocksMap.insert(coordToIndex(block->data(Block_Coord).toPoint()), block);
    frozenBlocks.append(block);
  }
  //blocks were frozen, now remove and delete the actual piece group (which is also an item)
  //removeItem(piece);
  //remember to remove it from our list as well
  activePieces.removeAll(piece);
  //and finally delete it (removeItem gaves us ownership of it, according to the docs)
  delete piece;
  searchForCompleteLines();
}

void KBlocksScene::searchForCompleteLines()
{
  QList<int> frozenBlocksMap;
  foreach (QGraphicsItem *block, frozenBlocks) {
    QPoint checkcoord = block->data(Block_Coord).toPoint();
    frozenBlocksMap << coordToIndex(checkcoord);
  }
  
  QList<int> linesToRemove;
  //We are scanning top to bottom, and analyzing the whole field to see which lines are complete
  for (int y=0; y<FIELD_HEIGHT; y++) {
    bool lineComplete = true;
    for (int x=0; x<FIELD_WIDTH; x++) {
      if (!frozenBlocksMap.contains(coordToIndex(QPoint(x,y)))) {
        lineComplete = false;
      }
    }
    if (lineComplete) {
      //Add this line to our list
      linesToRemove << y; 
    }
  }
  //Remove the lines detected as completed, one at a time
  foreach (int liney, linesToRemove) {
    removeLine(liney);
  }
}

void KBlocksScene::removeLine(int liney)
{
  //First we remove all blocks in the line
  QList<Block *> fadeOutBlocks;
  //QPixmap outpix = renderElement(BLOCK_SIZE, BLOCK_SIZE, QString("BLOCK_OUT_0"));
  foreach (Block *block, frozenBlocks) {
    QPoint checkcoord = block->data(Block_Coord).toPoint();
    if (checkcoord.y()==liney) {
      frozenBlocks.removeAll(block);
      int color = block->data(Block_Color).toInt();
      block->setPixmap(renderElement(BLOCK_SIZE, BLOCK_SIZE, QString("BLOCK_OUT_%1").arg(color)));
      //block->setElementId(QString("BLOCK_OUT_0"));
      fadeOutBlocks << block;
      //block->hide();
      //removeItem(block);
      //delete block;
    }
  }
  //FadeOut animator also removes blocks from scene and deletes them when it is done :)
  FadeAnimator * fadeOutAnim = new FadeAnimator(fadeOutBlocks, 200, QTimeLine::Backward, true);
  animators << fadeOutAnim;
  connect(fadeOutAnim, SIGNAL(finished(QObject *)), SLOT(animationFinished(QObject *)) );
  //Now we drop all blocks above this line, one line down
  //First we make a big list of all the blocks we will drop
  QList<Block *> dropBlocks;
  foreach (Block *block, frozenBlocks) {
    QPoint checkcoord = block->data(Block_Coord).toPoint();
    if (checkcoord.y()<liney) {
      dropBlocks << block;
    }
  }
  //Now we drop them
  /*foreach (Block *block, dropBlocks) {
    moveBlock(block, QPoint(0,1));
  }*/
  foreach (Block *block, dropBlocks) {
    //Set the logical data in the block corresponding to the next position in the grid
    QPoint curcoord = block->data(Block_Coord).toPoint();
    QPoint finalcoord = curcoord + QPoint(0,1);
    block->setData(Block_Coord, finalcoord);
    //Now prepare animation, sending actual scene coordinates
    if (block->animatePosSetup(coordToPoint(curcoord),coordToPoint(finalcoord))) {
      //block was already animating position, remove it from previous animator
      foreach (QObject *animator, animators) {
        DropAnimator * thisanim = (DropAnimator *) animator;
        thisanim->removeBlock(block);
      }
    }
  }
  DropAnimator * dropAnim = new DropAnimator(dropBlocks, 200, QTimeLine::Forward, false);
  animators << dropAnim;
  connect(dropAnim, SIGNAL(finished(QObject *)), SLOT(animationFinished(QObject *)) );
}

void KBlocksScene::animationFinished(QObject * animation)
{
  animators.removeAll(animation);
  delete animation;
}

int KBlocksScene::coordToIndex(const QPoint& coord)
{
  return coord.x()+(coord.y()*FIELD_WIDTH);
}

QPoint KBlocksScene::indexToCoord(int idx)
{
  QPoint point(idx%FIELD_WIDTH, idx/FIELD_WIDTH);
  return point;
}

QPointF KBlocksScene::coordToPoint(const QPoint& coord)
{
  QPointF point(coord.x(),coord.y());
  point = point*BLOCK_SIZE;
  point = point+fieldOffset;
  return point;
}

void KBlocksScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //releasePiece();
    QGraphicsScene::mousePressEvent(event);
}

void KBlocksScene::keyPressEvent(QKeyEvent *event)
{
  /*if (we do not want to handle it) {
    return;
  }*/
    switch (event->key()) {
      case Qt::Key_Left:
        attemptMove(QPoint(-1,0));
        break;
      case Qt::Key_Right:
        attemptMove(QPoint(1,0));
        break;
      case Qt::Key_Down:
        attemptMove(QPoint(0,1));
        break;
      case Qt::Key_Up:
        attemptRotation();
        break;
      case Qt::Key_Space:
        attemptMove(QPoint(0,1));
        break;
    }
}

void KBlocksScene::initPieceTypes()
{
  PieceRotation apiece;
  PieceSet aset;

  //Four blocks, straight line (I)
  apiece << QPoint(-1,0) << QPoint(0,0) << QPoint(1,0) << QPoint(2,0);
  aset << apiece;
  apiece.clear();
  apiece << QPoint(0,-1) << QPoint(0,0) << QPoint(0,1) << QPoint(0,2);
  aset << apiece;
  apiece.clear();
  pieceTypes << aset;
  aset.clear();

  //Four blocks, resembling an airplane (T)
  apiece << QPoint(0,0) << QPoint(0,-1) << QPoint(1,0) << QPoint(-1,0);
  aset << apiece;
  apiece.clear();
  apiece << QPoint(0,0) << QPoint(0,-1) << QPoint(-1,0) << QPoint(0,1);
  aset << apiece;
  apiece.clear();
  apiece << QPoint(0,0) << QPoint(1,0) << QPoint(-1,0) << QPoint(0,1); 
  aset << apiece;
  apiece.clear();
  apiece << QPoint(0,0) << QPoint(0,-1) << QPoint(1,0) << QPoint(0,1);
  aset << apiece;
  apiece.clear();
  pieceTypes << aset;
  aset.clear();

  //Four blocks, stair-like (S)
  apiece << QPoint(0,0) << QPoint(0,1) << QPoint(1,0) << QPoint(1,-1);
  aset << apiece;
  apiece.clear();
  apiece << QPoint(0,0) << QPoint(-1,0) << QPoint(0,1) << QPoint(1,1);
  aset << apiece;
  apiece.clear();
  pieceTypes << aset;
  aset.clear();

  //Four blocks, also stair-like (Z)
  apiece << QPoint(0,0) << QPoint(0,-1) << QPoint(1,0) << QPoint(1,1);
  aset << apiece;
  apiece.clear();
  apiece << QPoint(0,0) << QPoint(-1,0) << QPoint(0,-1) << QPoint(1,-1);
  aset << apiece;
  apiece.clear();
  pieceTypes << aset;
  aset.clear();

  //Four blocks, L shape (L)
  apiece << QPoint(0,0) << QPoint(0,-1) << QPoint(0,-2) << QPoint(1,0);
  aset << apiece;
  apiece.clear();
  apiece << QPoint(0,0) << QPoint(-1,0) << QPoint(-2,0) << QPoint(0,-1);
  aset << apiece;
  apiece.clear();
  apiece << QPoint(0,0) << QPoint(0,1) << QPoint(0,2) << QPoint(-1,0);
  aset << apiece;
  apiece.clear();
  apiece << QPoint(0,0) << QPoint(1,0) << QPoint(2,0) << QPoint(0,1);
  aset << apiece;
  apiece.clear();
  pieceTypes << aset;
  aset.clear();

  //Four blocks, also (J)
  apiece << QPoint(0,0) << QPoint(0,-1) << QPoint(0,-2) << QPoint(-1,0);
  aset << apiece;
  apiece.clear();
  apiece << QPoint(0,0) << QPoint(-1,0) << QPoint(-2,0) << QPoint(0,1);
  aset << apiece;
  apiece.clear();
  apiece << QPoint(0,0) << QPoint(0,1) << QPoint(0,2) << QPoint(1,0);
  aset << apiece;
  apiece.clear();
  apiece << QPoint(0,0) << QPoint(1,0) << QPoint(2,0) << QPoint(0,-1);
  aset << apiece;
  apiece.clear();
  pieceTypes << aset;
  aset.clear();

  //Four blocks, square (O)
  apiece << QPoint(0,0) << QPoint(1,0) << QPoint(0,-1) << QPoint(1,-1);
  aset << apiece;
  apiece.clear();
  pieceTypes << aset;
  aset.clear();
}

QPixmap KBlocksScene::renderElement(short width, short height, const QString & elementid) {
  QImage qiRend(QSize(width, height),QImage::Format_ARGB32_Premultiplied);
  qiRend.fill(0);

  if (renderer->isValid()) {
    QPainter p(&qiRend);
    renderer->render(&p, elementid);
  }
  return QPixmap::fromImage(qiRend);
}

void KBlocksScene::showMessage( const QString& message, int ms )
{
  messageItem->setMessageTimeout( ms );
  messageItem->showMessage( message, KGamePopupItem::TopLeft );
}
