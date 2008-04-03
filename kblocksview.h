/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2007 Mauricio Piacentini <mauricio@tabuleiro.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
#ifndef KBLOCKSVIEW_H
#define KBLOCKSVIEW_H

#include <QGraphicsView>

class KBlocksScene;

class KBlocksView : public QGraphicsView
{
  Q_OBJECT
  public:
    KBlocksView (QWidget * parent = 0 );
    ~KBlocksView();
    QObject * getSceneObject() { return (QObject *) m_scene; }
    
  public slots:
    void newGame();
    void pauseGame(bool pressed);
    void settingsChanged();
    void rotateCW();
    void rotateCCW();
    void moveLeft();
    void moveRight();
    void moveDown();
    
  protected:
    void focusInEvent ( QFocusEvent * event );
    void focusOutEvent ( QFocusEvent * event );

  protected:
    KBlocksScene * m_scene;
    void resizeEvent(QResizeEvent* event);
};

#endif
