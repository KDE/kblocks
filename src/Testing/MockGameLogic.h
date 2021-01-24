/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/
#ifndef MOCKGAMELOGIC_H
#define MOCKGAMELOGIC_H

#include "GameLogicInterface.h"

class MockGameLogic : public GameLogicInterface
{
public:
    MockGameLogic()
        : singleGamesDeleted(false)
    {
    }
    virtual ~MockGameLogic() = default;

public:
    SingleGameInterface *getSingleGame(int) override { return nullptr; }

    int levelUpGame(int) override { return 0; }
    int updateGame(int *) override { return 0; }

    void setGameSeed(int) override {}

    void setGameStandbyMode(bool) override {}

    bool startGame(int) override { return true; }
    bool stopGame() override { return true; }

    void pauseGame(bool) override {}
    void continueGame() override {}

    bool deleteSingleGames() override { singleGamesDeleted = true; return true; }

public:
    bool singleGamesDeleted;
};

#endif //MOCKGAMELOGIC_H
