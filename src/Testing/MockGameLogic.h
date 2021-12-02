/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#ifndef MOCKGAMELOGIC_H
#define MOCKGAMELOGIC_H

#include "GameLogicInterface.h"
#include "Testing/MockSingleGame.h"

class MockGameLogic : public GameLogicInterface
{
public:
    MockGameLogic()
        : singleGamesDeleted(false),
          singleGame{}
    {
    }
    ~MockGameLogic() override = default;

public:
    SingleGameInterface *getSingleGame(int) override { return &singleGame; }

    int levelUpGame(int) override { return 0; }
    int updateGame(int *) override { return 0; }

    void setGameSeed(int) override {}

    void setGameStandbyMode(bool) override {}

    bool startGame(int) override { return true; }
    bool stopGame() override { return true; }

    void pauseGame(bool) override {}
    void continueGame() override {}

    bool hasSingleGames() override { return true; }
    void deleteSingleGames() override { singleGamesDeleted = true; }

public:
    bool singleGamesDeleted;

private:
    MockSingleGame singleGame;
};

#endif //MOCKGAMELOGIC_H
