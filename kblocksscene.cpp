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
#include "kblocksgraphics.h"
#include <QtDebug>
#include <KStandardDirs>
#include <KGamePopupItem>

#include "settings.h"

const int INITIAL_UPDATE_INTERVAL = 400;

KBlocksScene::KBlocksScene() : gameState(Game_Starting), currentLevel(0), currentPoints(0), currentRemovedLines(0), inLockPosition(false), updateInterval(INITIAL_UPDATE_INTERVAL)
{
    initPieceTypes();
    nextPiece = new Piece();
    QString themeFile(Settings::theme());
    grafx = new KBlocksGraphics(themeFile);
    setSceneRect(0, 0, grafx->m_View_Size_Width, grafx->m_View_Size_Height);

    //playArea is our first item, non parented. We add it explicitally.
    playArea = new QGraphicsSvgItem();
    playArea->setSharedRenderer(grafx->renderer());
    playArea->setElementId("VIEW");
    //playArea->setPixmap(grafx->elementPixmap(grafx->m_View_Size_Width, grafx->m_View_Size_Height, QString("VIEW")));
    addItem(playArea);

    //Our Message Item, hidden by default
    messageItem = new KGamePopupItem();
    messageItem->setMessageOpacity(0.9);
    addItem(messageItem);
    
    setItemIndexMethod(NoIndex);
    stepTimer.setInterval(updateInterval);
    connect(&stepTimer, SIGNAL(timeout()), SLOT(step()));
    
    releaseTimer.setSingleShot(true);
    connect(&releaseTimer, SIGNAL(timeout()), SLOT(releasePiece()));
    releaseTimer.stop();

    startGame();
}

KBlocksScene::~KBlocksScene()
{
  cleanAll();
  delete nextPiece;
  delete grafx;
}

void KBlocksScene::readSettings(const QSize & viewSize)
{
  if (grafx->theme()->fileName()!=Settings::theme())
  {
    grafx->loadTheme(Settings::theme());
    
    grafx->adjustForSize(viewSize);
    updateDimensions();
  }
}

void KBlocksScene::drawBackground ( QPainter * painter, const QRectF & rect )
{
  if (grafx->renderer()->isValid()) {
    grafx->renderer()->render(painter, QString("BACKGROUND"), rect);
  }
}

void KBlocksScene::viewScaled(const QSize& newsize)
{
  //Temporarily halt game timer while resizing elements
  //if (gameState==Game_Active) stepTimer.stop();
  //grafx->adjustForSize(newsize);
  //updateDimensions();
  //Do not restart if game was paused
  //if (gameState==Game_Active) stepTimer.start();
}

void KBlocksScene::updateDimensions()
{
  setSceneRect(0, 0, grafx->m_View_Size_Width, grafx->m_View_Size_Height);
  //return;
  //playArea->setPixmap(grafx->elementPixmap(grafx->m_View_Size_Width, grafx->m_View_Size_Height, QString("VIEW")));
  playArea->setSharedRenderer(grafx->renderer());
  playArea->setElementId("VIEW");
  
  //TODO: calculate font size
  //scoreArea->setPos(grafx->m_ScoreArea_OffsetPoint_X, grafx->m_ScoreArea_OffsetPoint_Y);
  //levelArea->setPos(grafx->m_LevelArea_OffsetPoint_X, grafx->m_LevelArea_OffsetPoint_Y);

  foreach (Piece* piece, activePieces) {
    foreach (Block *block, piece->children()) {
      //block->setPixmap(grafx->elementPixmap(grafx->m_Block_Size, grafx->m_Block_Size, QString("BLOCK_%1").arg(block->data(Block_Color).toInt())));
      block->setSharedRenderer(grafx->renderer());
      block->setElementId(QString("BLOCK_%1").arg(block->data(Block_Color).toInt()));
      block->setPos(coordToPoint(block->data(Block_Coord).toPoint()));
    }
  }
  foreach (Block *block, nextPiece->children()) {
    //block->setPixmap(grafx->elementPixmap(grafx->m_Block_Size, grafx->m_Block_Size, QString("BLOCK_%1").arg(block->data(Block_Color).toInt())));
    block->setSharedRenderer(grafx->renderer());
    block->setElementId(QString("BLOCK_%1").arg(block->data(Block_Color).toInt()));
    block->setPos(nextPieceCoordToPoint(block->data(Block_Coord).toPoint()));
  }
  centerPiecePreview(nextPiece);
  
  foreach (Block* block, frozenBlocks) {
    //block->setPixmap(grafx->elementPixmap(grafx->m_Block_Size, grafx->m_Block_Size, QString("BLOCK_%1").arg(block->data(Block_Color).toInt())));
    block->setSharedRenderer(grafx->renderer());
    block->setElementId(QString("BLOCK_%1").arg(block->data(Block_Color).toInt()));
    block->setPos(coordToPoint(block->data(Block_Coord).toPoint()));
  }
}

void KBlocksScene::step()
{
    foreach (Piece* piece, activePieces) {
      //check if we can move down
      if (canMove(piece, QPoint(0,1))) {
        movePiece(piece, QPoint(0,1));
      } else {
        //maybe approaching lock
        if (!inLockPosition) {
          inLockPosition = true;
          return;
        }
        //Has it ended?
        if (isTrappedAtTop(piece)) {
          QString end("Game Over"); 
          showMessage( end, 4000 );
          stepTimer.stop();
          gameState=Game_Finished;
        } else {
          //we hit something, stop and detach
          freezePiece(piece);
          int linesRemoved = searchForCompleteLines();
          releaseTimer.start(linesRemoved*200);
          inLockPosition = false;
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
  //Classical games start at level 0
  currentLevel = 0;
  currentRemovedLines = 0;
  currentPoints = 0;
  inLockPosition = false;
  updateInterval = INITIAL_UPDATE_INTERVAL;
  stepTimer.setInterval(updateInterval);
  gameState=Game_Active;
    //Fire the first piece in two seconds
  releaseTimer.start(2000);
  QTimer::singleShot(500, this, SLOT(greetPlayer())); 
}

void KBlocksScene::greetPlayer()
{
   QString start("Game started"); 
   showMessage( start, 2000 ); 
}

void KBlocksScene::pauseGame(bool pause, bool fromUI)
{
  //Only work for paused, suspended and active states
  /*if ((gameState!=Game_Paused)&&(gameState!=Game_Active) &&(gameState!=Game_Suspended))
    return;*/
  
  if (!fromUI) {
   //not user initiated, check if we must suspend or resume
    if (pause) {
      previousGameState = gameState;
      gameState = Game_Suspended;
      stepTimer.stop();
    } else {
      gameState = previousGameState;
      if (gameState==Game_Active) stepTimer.start();
      if (gameState==Game_Paused) stepTimer.stop();
    }
    return;
  }
  
  if ((gameState==Game_Paused)&&!pause)  {
    stepTimer.start();
    QString end("Resuming Game"); 
    showMessage( end, 2000 );
    gameState=Game_Active;
  } else if ((gameState==Game_Active)&&pause){
    QString end("Game Paused"); 
    showMessage( end, 2000 );
    stepTimer.stop();
    gameState=Game_Paused;
  } else {
    //inconsistency, restore state and log
    kDebug()<<"Inconsistent Game State at pauseGame:"<<gameState<<pause;
  }
}

void KBlocksScene::levelUp()
{
  currentLevel++;
  //TODO: fine tune update interval variation based on play testing
  updateInterval -= 25;
  //Lock minimal drop rate
  if (updateInterval < 50) {
    updateInterval = 25;
  }
  stepTimer.setInterval(updateInterval);
  //kDebug(1100) << "Update interval is now: " << updateInterval;
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

void KBlocksScene::attemptRotation(KBlocksRotationDirection direction)
{
  foreach (Piece* piece, activePieces) {
      //check if we can rotate
    if (canRotate(piece, direction)) {
      rotatePiece(piece, direction);
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
    block->setSharedRenderer(grafx->renderer());
    block->setElementId(QString("BLOCK_%1").arg(chosenset));
    //block->setPixmap(grafx->elementPixmap(grafx->m_Block_Size, grafx->m_Block_Size, QString("BLOCK_%1").arg(chosenset)));
    block->setData(Block_OffsetInPiece, chosenpiecerotation.at(i));
    block->setData(Block_Color, chosenset);
    QPoint point = chosenpiecerotation.at(i);
    block->setData(Block_Coord, point);
    //Position the block initially in the preview area
    block->setPos(nextPieceCoordToPoint(point));
      //and append them to temporary collection
    nextPiece->addItem(block);
  }
  //Store blueprint data needed to recreate/rotate the piece
  nextPiece->setData(Piece_Set, chosenset);
  nextPiece->setData(Piece_Rotation, chosenorientation);
  
  //Adjust the position of the blocks so that the piece is nicely centered in the preview area
  centerPiecePreview(nextPiece);
  
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
      block->setSharedRenderer(grafx->renderer());
      block->setElementId(QString("BLOCK_%1").arg(setidx));
      //block->setPixmap(grafx->elementPixmap(grafx->m_Block_Size, grafx->m_Block_Size, QString("BLOCK_%1").arg(setidx)));

      block->setData(Block_OffsetInPiece, chosenpiecerotation.at(i));
      block->setData(Block_Color, setidx);
      QPoint point = chosenpiecerotation.at(i)+QPoint(grafx->m_PlayArea_NumberOfBlocks_X/2,0);
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
    if (piececoord.y() >= grafx->m_PlayArea_NumberOfBlocks_Y ) return false;
    //Or maybe the edges?
    if ((piececoord.x() >= grafx->m_PlayArea_NumberOfBlocks_X)||(piececoord.x() < 0)) return false;
    
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

bool KBlocksScene::canRotate(Piece * piece, KBlocksRotationDirection direction)
{
  //Retrieve pieceSet and orientation
  int setidx = piece->data(Piece_Set).toInt();
  int rotationidx = piece->data(Piece_Rotation).toInt();
  PieceSet pieceset = pieceTypes.at(setidx);
  PieceRotation curpiecerotation = pieceset.at(rotationidx);
  //get here current delta
  int nextrotationidx;
  if (direction==Rotate_Clockwise) {
    nextrotationidx = rotationidx+1;
    if (nextrotationidx >= pieceset.count()) {
      //wrap up next rotation
      nextrotationidx = 0;
    } 
  } else { //CCW rotation
    nextrotationidx = rotationidx-1;
    if (nextrotationidx < 0) {
      //wrap up next rotation
      nextrotationidx = pieceset.count() - 1;
    } 
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
    if (piececoord.y() >= grafx->m_PlayArea_NumberOfBlocks_Y ) return false;
    //Or maybe the edges?
    if ((piececoord.x() >= grafx->m_PlayArea_NumberOfBlocks_X)||(piececoord.x() < 0)) return false;
    
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

void KBlocksScene::rotatePiece(Piece * piece, KBlocksRotationDirection direction)
{
  //Retrieve pieceSet and orientation
  int setidx = piece->data(Piece_Set).toInt();
  int rotationidx = piece->data(Piece_Rotation).toInt();
  PieceSet pieceset = pieceTypes.at(setidx);
  PieceRotation curpiecerotation = pieceset.at(rotationidx);
  //get here current delta
  int nextrotationidx;
  if (direction==Rotate_Clockwise) {
    nextrotationidx = rotationidx+1;
    if (nextrotationidx >= pieceset.count()) {
      //wrap up next rotation
      nextrotationidx = 0;
    } 
  } else { //CCW rotation
    nextrotationidx = rotationidx-1;
    if (nextrotationidx < 0) {
      //wrap up next rotation
      nextrotationidx = pieceset.count() - 1;
    } 
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
}

int KBlocksScene::searchForCompleteLines()
{
  QList<int> frozenBlocksMap;
  foreach (QGraphicsItem *block, frozenBlocks) {
    QPoint checkcoord = block->data(Block_Coord).toPoint();
    frozenBlocksMap << coordToIndex(checkcoord);
  }
  
  QList<int> linesToRemove;
  //We are scanning top to bottom, and analyzing the whole field to see which lines are complete
  for (int y=0; y<  grafx->m_PlayArea_NumberOfBlocks_Y; y++) {
    bool lineComplete = true;
    for (int x=0; x<  grafx->m_PlayArea_NumberOfBlocks_X; x++) {
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
  //Score all lines at once, to allow combo values
  int linesRemoved = linesToRemove.count();
  if (linesRemoved>0) addToScore(Score_Lines, linesRemoved);
  
  return linesRemoved;
}

void KBlocksScene::removeLine(int liney)
{
  //First we remove all blocks in the line
  QList<Block *> fadeOutBlocks;
  foreach (Block *block, frozenBlocks) {
    QPoint checkcoord = block->data(Block_Coord).toPoint();
    if (checkcoord.y()==liney) {
      frozenBlocks.removeAll(block);
      int color = block->data(Block_Color).toInt();
      //block->setPixmap(grafx->elementPixmap(grafx->m_Block_Size, grafx->m_Block_Size, QString("BLOCK_OUT_%1").arg(color)));
      block->setElementId(QString("BLOCK_OUT_0"));
      fadeOutBlocks << block;
      //block->hide();
      //removeItem(block);
      //delete block;
    }
  }
  //FadeOut animator also removes blocks from scene and deletes them when it is done :)
  FadeAnimator * fadeOutAnim = new FadeAnimator(fadeOutBlocks, 600, QTimeLine::Backward, true);
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
  DropAnimator * dropAnim = new DropAnimator(dropBlocks, 600, QTimeLine::Forward, false);
  animators << dropAnim;
  connect(dropAnim, SIGNAL(finished(QObject *)), SLOT(animationFinished(QObject *)) );
}

void KBlocksScene::addToScore(KBlocksScoreEvent type, int count)
{
  switch (type) {
    case Score_Blocks:
      //TODO
      break;
    case Score_Lines:
      //compute value for combos (multiple lines removed at once)
      int comboLinesValue;
      switch (count) {
        case 1: 
          comboLinesValue = 40;
          break;
        case 2: 
          comboLinesValue = 100;
          break;
        case 3: 
          comboLinesValue = 300;
          break;
        case 4: 
          comboLinesValue = 1200;
          break;
      }
      //Classical scoring and gameplay uses level 0 as the first one
      currentPoints = currentPoints + (comboLinesValue*(currentLevel+1));
      currentRemovedLines += count;
      //test for levelUp
      if ((currentRemovedLines/10) >= (currentLevel+1)) {
        levelUp();
      }
      break;
    case Score_Level:
      //TODO
      break;
  }
  //scoreArea->setPlainText(QString("%1").arg(currentPoints));
  //levelArea->setPlainText(QString("%1").arg(currentLevel));
  kDebug(11000) << "Points:" << currentPoints << "Lines:" << currentRemovedLines << "Level:" << currentLevel;
}

void KBlocksScene::animationFinished(QObject * animation)
{
  animators.removeAll(animation);
  animation->deleteLater();
}

int KBlocksScene::coordToIndex(const QPoint& coord)
{
  return coord.x()+(coord.y()*grafx->m_PlayArea_NumberOfBlocks_X);
}

QPoint KBlocksScene::indexToCoord(int idx)
{
  int numblocksx = grafx->m_PlayArea_NumberOfBlocks_X;	
  QPoint point(idx%numblocksx, idx/numblocksx);
  return point;
}

QPointF KBlocksScene::coordToPoint(const QPoint& coord)
{
  QPointF point(coord.x(),coord.y());
  point = point*grafx->m_Block_Size;
  point = point+QPointF(grafx->m_PlayArea_OffsetPoint_X,grafx->m_PlayArea_OffsetPoint_Y);
  return point;
}

QPointF KBlocksScene::nextPieceCoordToPoint(const QPoint& coord)
{
  QPointF point(coord.x(),coord.y());
  point = point*grafx->m_Block_Size;
  point = point+QPointF(grafx->m_PreviewArea_CenterPoint_X,grafx->m_PreviewArea_CenterPoint_Y);
  return point;
}

QPointF KBlocksScene::pieceCenterPoint(Piece * piece)
{
  qreal minX=0.0;
  qreal minY=0.0;
  qreal maxX=0.0;
  qreal maxY=0.0;
  foreach (Block *block, piece->children()) {
    QPoint coord = block->data(Block_Coord).toPoint();
    if (coord.x() < minX) minX = (qreal) coord.x();
    if (coord.y() < minY) minY = (qreal) coord.y();
    if (coord.x() > maxX) maxX = (qreal) coord.x();
    if (coord.y() > maxY) maxY = (qreal) coord.y();
  }
  QPointF center((minX+maxX)/2.0, (minY+maxY)/2.0);
  return center;
}

void KBlocksScene::centerPiecePreview(Piece * piece)
{
  //Now we will adjust our position so that the piece is nicely centered in the preview area
  //no matter the orientation or size of it
  //This assumes the piece has already been placed in the previewArea (using nextPieceCoordToPoint)
  //Find out pieceCenterPoint
  QPointF centerPoint = pieceCenterPoint(piece);
  centerPoint = centerPoint*grafx->m_Block_Size;
  //Remember that QGV coordinate system takes the top left of the element for pixmaps, so add 1/2 block size
  centerPoint = centerPoint+QPointF(grafx->m_Block_Size/2, grafx->m_Block_Size/2);
  //Now reposition all blocks
  foreach (Block *block, piece->children()) {
    block->setPos(block->pos() - centerPoint);
  }
}

void KBlocksScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //releasePiece();
    QGraphicsScene::mousePressEvent(event);
}

void KBlocksScene::keyPressEvent(QKeyEvent *event)
{
    if (gameState!=Game_Active) {
      return;
    }
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
        attemptRotation(Rotate_Clockwise);
        break;
      case Qt::Key_Z:
        attemptRotation(Rotate_CounterClockwise);
        break;
      case Qt::Key_X:
        attemptRotation(Rotate_Clockwise);
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

void KBlocksScene::showMessage( const QString& message, int ms )
{
  messageItem->setMessageTimeout( ms );
  messageItem->showMessage( message, KGamePopupItem::TopLeft );
}
