/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#ifndef GAME_LOGIC_INTERFACE
#define GAME_LOGIC_INTERFACE

#include <QObject>

#include "SingleGameInterface.h"

class GameLogicInterface : public QObject
{
    Q_OBJECT
public:
    GameLogicInterface();
    ~GameLogicInterface() override {};

public:
    virtual SingleGameInterface *getSingleGame(int) = 0;

    virtual int levelUpGame(int) = 0;
    virtual int updateGame(int *) = 0;

    virtual void setGameSeed(int) = 0;

    virtual void setGameStandbyMode(bool) = 0;

    virtual bool startGame(int) = 0;
    virtual bool stopGame() = 0;

    virtual void pauseGame(bool) = 0;
    virtual void continueGame() = 0;

    virtual bool hasSingleGames() = 0;
    virtual void deleteSingleGames() = 0;

Q_SIGNALS:
    void allGamesStopped();

protected:
    SingleGameInterface **maGameList;
};

#endif //GAME_LOGIC_INTERFACE
