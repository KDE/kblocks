/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/
#ifndef MOCKSCENE_H
#define MOCKSCENE_H

#include "SceneInterface.h"
#include "Testing/MockGameLogic.h"

class MockScene : public SceneInterface
{
public:
    MockScene(MockGameLogic *gameLogic)
        : singleGameDeletedWhenStopGameCalled(false),
          readSettingsCalled(false),
          mpGameLogic(gameLogic)
    {
    }
    virtual ~MockScene() = default;

public:
    void createGameItemGroups(int, bool) override {}
    void deleteGameItemGroups() override {}
    void setGamesPerLine(int) override {}
    void setGameAnimEnabled(bool) override {}
    void setWaitForAllUpdate(bool) override {}
    void setUpdateInterval(int) override {}
    void setSoundsEnabled(bool) override {}
    void readSettings(const QSize&) override { readSettingsCalled = true; }
    void startGame() override {}
    void stopGame() override {
        if (mpGameLogic->singleGamesDeleted) {
            singleGameDeletedWhenStopGameCalled = true;
        }
    }
    void pauseGame(bool, bool) override {}
    void addScore(int, int) override {}

public slots:
    void playMoveSound() override {}
    void playDropSound() override {}

public:
    bool singleGameDeletedWhenStopGameCalled;
    bool readSettingsCalled;

private:
    MockGameLogic *mpGameLogic;
};

#endif // MOCKSCENE_H
