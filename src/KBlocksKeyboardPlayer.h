/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#ifndef KBLOCKSKEYBOARDPLAYER_H
#define KBLOCKSKEYBOARDPLAYER_H

#include <string>
using namespace std;

#include <QAction>
#include <KActionCollection>
#include <KXmlGuiWindow>
#include <KLocalizedString>

#include "GamePlayerInterface.h"

class KBlocksKeyboardPlayer : public QObject, public GamePlayerInterface
{
    Q_OBJECT

public:
    explicit KBlocksKeyboardPlayer(KXmlGuiWindow *parent, const string &name = "", bool netMode = false);
    ~KBlocksKeyboardPlayer() override;

public:
    void startGame(SingleGameInterface *p) override;
    void stopGame() override;

    void pauseGame(bool flag) override;

    void think(GamePlayer_ActionList *actionList) override;

    string getName() override;

private:
    void bindKeys();

Q_SIGNALS:
    void blockMoved();
    void blockDropped();

private Q_SLOTS:
    void moveLeft();
    void moveRight();
    void moveDown();
    void pushDown();
    void rotateCW();
    void rotateCCW();

protected:
    SingleGameInterface *mpGame = nullptr;
    bool mPauseFlag;

    QAction *rotatecw = nullptr;
    QAction *rotateccw = nullptr;
    QAction *moveleft = nullptr;
    QAction *moveright = nullptr;
    QAction *movedown = nullptr;
    QAction *pushdown = nullptr;

private:
    bool mNetMode;
    string mPlayerName;
    GamePlayer_ActionList mActionList;

    KXmlGuiWindow *mpKeyWindow = nullptr;
    KActionCollection *mpKeyShortcuts = nullptr;
};

#endif

