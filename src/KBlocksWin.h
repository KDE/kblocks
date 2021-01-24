/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2009-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/
#ifndef KBLOCKSWIN_H
#define KBLOCKSWIN_H

#include <KXmlGuiWindow>

#include "KBlocksView.h"

#include "KBlocksKeyboardPlayer.h"
#include "AI/KBlocksAIPlayer.h"

#include "KBlocksPlayManager.h"
class QLabel;
class GameLogicInterface;
class GraphicsInterface;
class SceneInterface;
class SoundInterface;
class KBlocksWin : public KXmlGuiWindow
{
    Q_OBJECT

public:
    KBlocksWin(
        GameLogicInterface *p,
        GraphicsInterface *graphics,
        SoundInterface *sound,
        KBlocksPlayManager *pM,
        int capacity,
        int gamecount
    );
    ~KBlocksWin();

public:
    void setGamesPerLine(int count);
    void setGameAnimEnabled(bool flag);
    void setWaitForAllUpdate(bool flag);
    void setUpdateInterval(int interval);

    void addScore(int gameIndex, int lineCount);

    void setupGUILayout();

protected slots:
    void stopGame();

private slots:
    void startGame();
    void pauseGame();

    void singleGame();
    void pveStepGame();

    void focusEvent(bool flag);

    void configureSettings();
    void showHighscore();
    void onScoreChanged(int index, int points, int lines, int level);
    void onIsHighscore(int index, int points, int level);
    void onAllGamesStopped();
    void levelChanged();
    void setSoundsEnabled(bool enabled);

protected:
    void closeEvent(QCloseEvent *event) override;

protected:
    SceneInterface *mpGameScene = nullptr;
    KBlocksView  *mpGameView = nullptr;

    KBlocksKeyboardPlayer *mpKBPlayer = nullptr;

private:
    int mMaxGameCapacity;
    int mGameCount;

    bool mGameAnim;
    bool mWaitForAll;

    GameLogicInterface *mpGameLogic = nullptr;
    KBlocksPlayManager *mpPlayManager = nullptr;

    KBlocksAIPlayer *mpAIPlayer = nullptr;

    QAction *m_pauseAction = nullptr;
    QLabel *mScore = nullptr;
};

#endif
