/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2009-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/

#ifndef SCENEINTERFACE_H
#define SCENEINTERFACE_H

#include <QGraphicsScene>

class SceneInterface : public QGraphicsScene
{
    Q_OBJECT
public:
    SceneInterface();
    virtual ~SceneInterface() = default;

public:
    virtual void createGameItemGroups(int, bool snapshotMode = false) = 0;
    virtual void deleteGameItemGroups() = 0;
    virtual void setGamesPerLine(int) = 0;
    virtual void setGameAnimEnabled(bool) = 0;
    virtual void setWaitForAllUpdate(bool) = 0;
    virtual void setUpdateInterval(int) = 0;
    virtual void setSoundsEnabled(bool) = 0;
    virtual void readSettings(const QSize&) = 0;
    virtual void startGame() = 0;
    virtual void stopGame() = 0;
    virtual void pauseGame(bool, bool fromUI = false) = 0;
    virtual void addScore(int, int) = 0;

signals:
    void scoreChanged(int index, int points, int lines, int level);
    void isHighscore(int index, int points, int level);

public slots:
    virtual void playMoveSound() = 0;
    virtual void playDropSound() = 0;
};

#endif // SCENEINTERFACE_H
