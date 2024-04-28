/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#include "KBlocksKeyboardPlayer.h"
#include <QIcon>
KBlocksKeyboardPlayer::KBlocksKeyboardPlayer(KXmlGuiWindow *parent, const string &name, bool netMode)
{
    mpGame = nullptr;
    mPauseFlag = false;
    mpKeyWindow = nullptr;

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
    mpGame = nullptr;

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
    rotatecw->setText(i18nc("@action", "Rotate Piece Clockwise"));
    rotatecw->setIcon(QIcon::fromTheme(QStringLiteral("object-rotate-right")));
    KActionCollection::setDefaultShortcut(rotatecw, Qt::Key_Z);
    connect(rotatecw, &QAction::triggered, this, &KBlocksKeyboardPlayer::rotateCW);

    rotateccw = mpKeyShortcuts->addAction(QStringLiteral("rotate_ccw"));
    rotateccw->setText(i18nc("@action", "Rotate Piece Counterclockwise"));
    rotateccw->setIcon(QIcon::fromTheme(QStringLiteral("object-rotate-left")));
    KActionCollection::setDefaultShortcut(rotateccw, Qt::Key_Up);
    connect(rotateccw, &QAction::triggered, this, &KBlocksKeyboardPlayer::rotateCCW);

    moveleft = mpKeyShortcuts->addAction(QStringLiteral("move_left"));
    moveleft->setText(i18nc("@action", "Move Piece Left"));
    moveleft->setIcon(QIcon::fromTheme(QStringLiteral("arrow-left")));
    KActionCollection::setDefaultShortcut(moveleft, Qt::Key_Left);
    connect(moveleft, &QAction::triggered, this, &KBlocksKeyboardPlayer::moveLeft);

    moveright = mpKeyShortcuts->addAction(QStringLiteral("move_right"));
    moveright->setText(i18nc("@action", "Move Piece Right"));
    moveright->setIcon(QIcon::fromTheme(QStringLiteral("arrow-right")));
    KActionCollection::setDefaultShortcut(moveright, Qt::Key_Right);
    connect(moveright, &QAction::triggered, this, &KBlocksKeyboardPlayer::moveRight);

    movedown = mpKeyShortcuts->addAction(QStringLiteral("move_down"));
    movedown->setText(i18nc("@action", "Move Piece Down"));
    movedown->setIcon(QIcon::fromTheme(QStringLiteral("arrow-down")));
    KActionCollection::setDefaultShortcut(movedown, Qt::Key_Down);
    connect(movedown, &QAction::triggered, this, &KBlocksKeyboardPlayer::moveDown);

    pushdown = mpKeyShortcuts->addAction(QStringLiteral("push_down"));
    pushdown->setText(i18nc("@action", "Drop the Piece"));
    pushdown->setIcon(QIcon::fromTheme(QStringLiteral("arrow-down")));
    KActionCollection::setDefaultShortcut(pushdown, Qt::Key_Space);
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
    Q_EMIT blockMoved();
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
    Q_EMIT blockMoved();
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
    Q_EMIT blockMoved();
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
    Q_EMIT blockDropped();
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
    Q_EMIT blockMoved();
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
    Q_EMIT blockMoved();
}

#include "moc_KBlocksKeyboardPlayer.cpp"
