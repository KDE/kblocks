/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2009-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/

#ifndef SCENEINTERFACE_H
#define SCENEINTERFACE_H

#include <QGraphicsScene>

class KBlocksTheme;

class SceneInterface : public QGraphicsScene
{
    Q_OBJECT
public:
    SceneInterface();
    ~SceneInterface() override = default;

public:
    virtual void createGameItemGroups(int, bool snapshotMode = false) = 0;
    virtual void deleteGameItemGroups() = 0;
    virtual void setGamesPerLine(int) = 0;
    virtual void setGameAnimEnabled(bool) = 0;
    virtual void setWaitForAllUpdate(bool) = 0;
    virtual void setUpdateInterval(int) = 0;
    virtual void setSoundsEnabled(bool) = 0;
    virtual void readSettings() = 0;
    virtual void loadTheme(const KBlocksTheme *theme) = 0;
    virtual void startGame() = 0;
    virtual void stopGame() = 0;
    virtual void pauseGame(bool, bool fromUI = false) = 0;
    virtual void addScore(int, int) = 0;

Q_SIGNALS:
    void scoreChanged(int index, int points, int lines, int level);
    void isHighscore(int index, int points, int level);

public Q_SLOTS:
    virtual void playMoveSound() = 0;
    virtual void playDropSound() = 0;
};

#endif // SCENEINTERFACE_H
