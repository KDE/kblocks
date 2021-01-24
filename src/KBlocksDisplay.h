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
#ifndef KBLOCKSDISPLAY_H
#define KBLOCKSDISPLAY_H

#include <string>

#include <KMainWindow>
#include <QTimer>

#include "KBlocksScene.h"
#include "KBlocksView.h"

#include "KBlocksGameLogic.h"
#include "KBlocksNetClient.h"

using std::string;

class QLabel;
class GraphicsInterface;
class SoundInterface;

class KBlocksDisplay : public KMainWindow
{
    Q_OBJECT

public:
    KBlocksDisplay(
        GraphicsInterface* graphics,
        SoundInterface* sound,
        int gameCount,
        const std::string& serverIP,
        int localPort
    );
    ~KBlocksDisplay();

public:
    void setGamesPerLine(int count);
    void setUpdateInterval(int interval);

public:
    void startDisplay();
    void stopDisplay();

private:
    int  formIntFromByte(char *data);
    void updateScore();

private slots:
    void updateEvent();
    void updateGameDisplay(int size);

private:
    int mGameCount;
    //int mGamesPerWidth;

    int mUpdateInterval;
    QTimer mUpdateTimer;

    int maScoreList[8];

    KBlocksScene *mpGameScene = nullptr;
    KBlocksView  *mpGameView = nullptr;

    KBlocksGameLogic *mpGameLogic = nullptr;
    KBlocksNetClient *mpNetClient = nullptr;
    QLabel *mScore = nullptr;
};

#endif

