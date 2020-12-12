/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksKeyboardPlayer.h"
#include <QIcon>
KBlocksKeyboardPlayer::KBlocksKeyboardPlayer(KXmlGuiWindow *parent, const string &name, bool netMode)
{
    mpGame = 0;
    mPauseFlag = false;
    mpKeyWindow = 0;

    mPlayerName = name;
    mNetMode = netMode;

    if (parent) {
        mpKeyShortcuts = parent->actionCollection();
    } else {
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
    delete mpKeyWindow;
}

void KBlocksKeyboardPlayer::startGame(SingleGameInterface *p)
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

void KBlocksKeyboardPlayer::think(GamePlayer_ActionList *actionList)
{
    if (mNetMode) {
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
    rotatecw = mpKeyShortcuts->addAction(QStringLiteral("rotate_cw"));
    rotatecw->setText(i18n("Rotate Piece Clockwise"));
    rotatecw->setIcon(QIcon::fromTheme(QStringLiteral("object-rotate-right")));
    mpKeyShortcuts->setDefaultShortcut(rotatecw, Qt::Key_Z);
    connect(rotatecw, &QAction::triggered, this, &KBlocksKeyboardPlayer::rotateCW);

    rotateccw = mpKeyShortcuts->addAction(QStringLiteral("rotate_ccw"));
    rotateccw->setText(i18n("Rotate Piece Counter Clockwise"));
    rotateccw->setIcon(QIcon::fromTheme(QStringLiteral("object-rotate-left")));
    mpKeyShortcuts->setDefaultShortcut(rotateccw, Qt::Key_Up);
    connect(rotateccw, &QAction::triggered, this, &KBlocksKeyboardPlayer::rotateCCW);

    moveleft = mpKeyShortcuts->addAction(QStringLiteral("move_left"));
    moveleft->setText(i18n("Move Piece Left"));
    moveleft->setIcon(QIcon::fromTheme(QStringLiteral("arrow-left")));
    mpKeyShortcuts->setDefaultShortcut(moveleft, Qt::Key_Left);
    connect(moveleft, &QAction::triggered, this, &KBlocksKeyboardPlayer::moveLeft);

    moveright = mpKeyShortcuts->addAction(QStringLiteral("move_right"));
    moveright->setText(i18n("Move Piece Right"));
    moveright->setIcon(QIcon::fromTheme(QStringLiteral("arrow-right")));
    mpKeyShortcuts->setDefaultShortcut(moveright, Qt::Key_Right);
    connect(moveright, &QAction::triggered, this, &KBlocksKeyboardPlayer::moveRight);

    movedown = mpKeyShortcuts->addAction(QStringLiteral("move_down"));
    movedown->setText(i18n("Move Piece Down"));
    movedown->setIcon(QIcon::fromTheme(QStringLiteral("arrow-down")));
    mpKeyShortcuts->setDefaultShortcut(movedown, Qt::Key_Down);
    connect(movedown, &QAction::triggered, this, &KBlocksKeyboardPlayer::moveDown);

    pushdown = mpKeyShortcuts->addAction(QStringLiteral("push_down"));
    pushdown->setText(i18n("Drop the Piece"));
    pushdown->setIcon(QIcon::fromTheme(QStringLiteral("arrow-down")));
    mpKeyShortcuts->setDefaultShortcut(pushdown, Qt::Key_Space);
    connect(pushdown, &QAction::triggered, this, &KBlocksKeyboardPlayer::pushDown);
}

void KBlocksKeyboardPlayer::moveLeft()
{
    if ((!mpGame) || (mPauseFlag)) {
        return;
    }
    if (mNetMode) {
        mActionList.push_back(PlayerAction_Move_Left);
    } else {
        mpGame->setCurrentPiece(-1, 0, 0);
    }
    emit blockMoved();
}

void KBlocksKeyboardPlayer::moveRight()
{
    if ((!mpGame) || (mPauseFlag)) {
        return;
    }
    if (mNetMode) {
        mActionList.push_back(PlayerAction_Move_Right);
    } else {
        mpGame->setCurrentPiece(1, 0, 0);
    }
    emit blockMoved();
}

void KBlocksKeyboardPlayer::moveDown()
{
    if ((!mpGame) || (mPauseFlag)) {
        return;
    }
    if (mNetMode) {
        mActionList.push_back(PlayerAction_Move_Down);
    } else {
        mpGame->setCurrentPiece(0, 1, 0);
    }
    emit blockMoved();
}

void KBlocksKeyboardPlayer::pushDown()
{
    if ((!mpGame) || (mPauseFlag)) {
        return;
    }
    if (mNetMode) {
        mActionList.push_back(PlayerAction_Push_Down);
    } else {
        while (mpGame->setCurrentPiece(0, 1, 0)) ;
        mpGame->forceUpdateGame();
    }
    emit blockDropped();
}

void KBlocksKeyboardPlayer::rotateCW()
{
    if ((!mpGame) || (mPauseFlag)) {
        return;
    }
    if (mNetMode) {
        mActionList.push_back(PlayerAction_Rotate_CW);
    } else {
        mpGame->setCurrentPiece(0, 0, -1);
    }
    emit blockMoved();
}

void KBlocksKeyboardPlayer::rotateCCW()
{
    if ((!mpGame) || (mPauseFlag)) {
        return;
    }
    if (mNetMode) {
        mActionList.push_back(PlayerAction_Rotate_CCW);
    } else {
        mpGame->setCurrentPiece(0, 0, 1);
    }
    emit blockMoved();
}
