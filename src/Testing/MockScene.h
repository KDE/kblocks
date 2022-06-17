/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
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
    ~MockScene() override = default;

public:
    void createGameItemGroups(int, bool) override {}
    void deleteGameItemGroups() override {}
    void setGamesPerLine(int) override {}
    void setGameAnimEnabled(bool) override {}
    void setWaitForAllUpdate(bool) override {}
    void setUpdateInterval(int) override {}
    void setSoundsEnabled(bool) override {}
    void readSettings() override { readSettingsCalled = true; }
    void loadTheme(const KgTheme *) override {}
    void startGame() override {}
    void stopGame() override {
        if (mpGameLogic->singleGamesDeleted) {
            singleGameDeletedWhenStopGameCalled = true;
        }
    }
    void pauseGame(bool, bool) override {}
    void addScore(int, int) override {}

public Q_SLOTS:
    void playMoveSound() override {}
    void playDropSound() override {}

public:
    bool singleGameDeletedWhenStopGameCalled;
    bool readSettingsCalled;

private:
    MockGameLogic *mpGameLogic;
};

#endif // MOCKSCENE_H
