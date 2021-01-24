/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2010-2021 Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
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
    virtual ~GameLogicInterface() {};

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

signals:
    void allGamesStopped();

protected:
    SingleGameInterface **maGameList;
};

#endif //GAME_LOGIC_INTERFACE
