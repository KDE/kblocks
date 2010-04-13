/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksKeyboardPlayer.h"

KBlocksKeyboardPlayer::KBlocksKeyboardPlayer(KXmlGuiWindow * parent, string name, bool netMode)
{
    mpGame = 0;
    mPauseFlag = false;
    mpKeyWindow = 0;
    
    mPlayerName = name;
    mNetMode = netMode;
    
    if (parent)
    {
        mpKeyShortcuts = parent->actionCollection();
    }
    else
    {
        mpKeyWindow = new KXmlGuiWindow();
        mpKeyWindow->setupGUI();
        mpKeyWindow->setFixedWidth(192);
        mpKeyWindow->setFixedHeight(32);
        mpKeyWindow->show();
        
        mpKeyShortcuts = mpKeyWindow->actionCollection();
    }
    
    bindKeys();
    
    mActionList.clear();
}

KBlocksKeyboardPlayer::~KBlocksKeyboardPlayer()
{
    if (mpKeyWindow)
    {
        delete mpKeyWindow;
    }
}

void KBlocksKeyboardPlayer::startGame(SingleGameInterface * p)
{
    mpGame = p;
    mPauseFlag = false;
    
    mActionList.clear();
}

void KBlocksKeyboardPlayer::stopGame()
{
    mpGame = 0;
    
    mActionList.clear();
}

void KBlocksKeyboardPlayer::pauseGame(bool flag)
{
    mPauseFlag = flag;
}

void KBlocksKeyboardPlayer::think(GamePlayer_ActionList * actionList)
{
    if (mNetMode)
    {
        *actionList = mActionList;
        mActionList.clear();
    }
}

string KBlocksKeyboardPlayer::getName()
{
    return mPlayerName;
}

void KBlocksKeyboardPlayer::bindKeys()
{
    rotatecw = mpKeyShortcuts->addAction("rotate_cw");
    rotatecw->setText(i18n("Rotate Piece Clockwise"));
    rotatecw->setIcon(KIcon("object-rotate-right"));
    rotatecw->setShortcuts( KShortcut( "z" ) );
    connect(rotatecw, SIGNAL(triggered(bool)), this, SLOT(rotateCW()));
    
    rotateccw = mpKeyShortcuts->addAction("rotate_ccw");
    rotateccw->setText(i18n("Rotate Piece Counter Clockwise"));
    rotateccw->setIcon(KIcon("object-rotate-left"));
    rotateccw->setShortcuts( KShortcut( Qt::Key_Up  ) );
    connect(rotateccw, SIGNAL(triggered(bool)), this, SLOT(rotateCCW()));
    
    moveleft = mpKeyShortcuts->addAction("move_left");
    moveleft->setText(i18n("Move Piece Left"));
    moveleft->setIcon(KIcon("arrow-left"));
    moveleft->setShortcuts( KShortcut( Qt::Key_Left ) );
    connect(moveleft, SIGNAL(triggered(bool)), this, SLOT(moveLeft()));
    
    moveright = mpKeyShortcuts->addAction("move_right");
    moveright->setText(i18n("Move Piece Right"));
    moveright->setIcon(KIcon("arrow-right"));
    moveright->setShortcuts( KShortcut( Qt::Key_Right) );
    connect(moveright, SIGNAL(triggered(bool)), this, SLOT(moveRight()));
    
    movedown = mpKeyShortcuts->addAction("move_down");
    movedown->setText(i18n("Move Piece Down"));
    movedown->setIcon(KIcon("arrow-down"));
    movedown->setShortcuts( KShortcut( Qt::Key_Down ) );
    connect(movedown, SIGNAL(triggered(bool)), this, SLOT(moveDown()));
    
    pushdown = mpKeyShortcuts->addAction("push_down");
    pushdown->setText(i18n("Drop the Piece"));
    pushdown->setIcon(KIcon("arrow-down"));
    pushdown->setShortcuts( KShortcut( Qt::Key_Space ) );
    connect(pushdown, SIGNAL(triggered(bool)), this, SLOT(pushDown()));
}

void KBlocksKeyboardPlayer::moveLeft()
{
    if ((!mpGame) || (mPauseFlag))
    {
        return;
    }
    if (mNetMode)
    {
        mActionList.push_back(PlayerAction_Move_Left);
    }
    else
    {
        mpGame->setCurrentPiece(-1, 0, 0);
    }
}

void KBlocksKeyboardPlayer::moveRight()
{
    if ((!mpGame) || (mPauseFlag))
    {
        return;
    }
    if (mNetMode)
    {
        mActionList.push_back(PlayerAction_Move_Right);
    }
    else
    {
        mpGame->setCurrentPiece(1, 0, 0);
    }
}

void KBlocksKeyboardPlayer::moveDown()
{
    if ((!mpGame) || (mPauseFlag))
    {
        return;
    }
    if (mNetMode)
    {
        mActionList.push_back(PlayerAction_Move_Down);
    }
    else
    {
        mpGame->setCurrentPiece(0, 1, 0);
    }
}

void KBlocksKeyboardPlayer::pushDown()
{
    if ((!mpGame) || (mPauseFlag))
    {
        return;
    }
    if (mNetMode)
    {
        mActionList.push_back(PlayerAction_Push_Down);
    }
    else
    {
        while(mpGame->setCurrentPiece(0, 1, 0)) ;
        mpGame->forceUpdateGame();
    }
}

void KBlocksKeyboardPlayer::rotateCW()
{
    if ((!mpGame) || (mPauseFlag))
    {
        return;
    }
    if (mNetMode)
    {
        mActionList.push_back(PlayerAction_Rotate_CW);
    }
    else
    {
        mpGame->setCurrentPiece(0, 0, 1);
    }
}

void KBlocksKeyboardPlayer::rotateCCW()
{
    if ((!mpGame) || (mPauseFlag))
    {
        return;
    }
    if (mNetMode)
    {
        mActionList.push_back(PlayerAction_Rotate_CCW);
    }
    else
    {
        mpGame->setCurrentPiece(0, 0, -1);
    }
}
