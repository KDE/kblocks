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
#include "KBlocksDisplay.h"

#include "AI/KBlocksAILog.h"

#include <KLocalizedString>
#include <QStatusBar>
#include <QPixmapCache>
#include <QLabel>

class GraphicsInterface;
class SoundInterface;

KBlocksDisplay::KBlocksDisplay(
    GraphicsInterface *graphics,
    SoundInterface *sound,
    int gameCount,
    const string &serverIP,
    int localPort
) : KMainWindow()
{
    //Use up to 3MB for global application pixmap cache
    QPixmapCache::setCacheLimit(3 * 1024);

    for (int i = 0; i < 8; ++i) {
        maScoreList[i] = 0;
    }

    mpNetClient = new KBlocksNetClient(serverIP.c_str(), localPort);
    connect(mpNetClient, &KBlocksNetClient::dataArrived, this, &KBlocksDisplay::updateGameDisplay);

    mGameCount = gameCount;
    mpGameLogic = new KBlocksGameLogic(mGameCount);
    mpGameLogic->setGameSeed(0);
    mpGameLogic->setGamePunish(false);
    mpGameLogic->setGameStandbyMode(true);
    mpGameLogic->setInitInterval(0);
    mpGameLogic->setLevelUpInterval(0);

    mpGameScene = new KBlocksScene(mpGameLogic, graphics, sound, mGameCount);
    mpGameScene->setGameAnimEnabled(false);
    mpGameScene->setWaitForAllUpdate(false);

    mpGameView = new KBlocksView(mpGameScene, this);
    mpGameView->show();
    setCentralWidget(mpGameView);

    mUpdateInterval = 1000;
    mUpdateTimer.setInterval(mUpdateInterval);
    connect(&mUpdateTimer, &QTimer::timeout, this, &KBlocksDisplay::updateEvent);
    mUpdateTimer.stop();
    mScore = new QLabel(i18n("Score List : 0 - 0 - 0 - 0 - 0 - 0 - 0 - 0"));
    statusBar()->addPermanentWidget(mScore);
}

KBlocksDisplay::~KBlocksDisplay()
{
    mpGameLogic->stopGame();
    delete mpGameLogic;

    delete mpGameView;
    delete mpGameScene;

    delete mpNetClient;
}

void KBlocksDisplay::setGamesPerLine(int count)
{
    mpGameScene->setGamesPerLine(count);
}

void KBlocksDisplay::setUpdateInterval(int interval)
{
    mUpdateInterval = interval;
    mUpdateTimer.setInterval(mUpdateInterval);
    mpGameScene->setUpdateInterval(interval);
}

void KBlocksDisplay::startDisplay()
{
    mpGameLogic->startGame(mGameCount);
    for (int i = 0; i < mGameCount; i++) {
        mpGameLogic->getSingleGame(i)->stopGame();
    }

    mpGameScene->createGameItemGroups(mGameCount, true);
    mpGameScene->startGame();

    mUpdateTimer.start();
}

void KBlocksDisplay::stopDisplay()
{
    mUpdateTimer.stop();

    mpGameScene->stopGame();
    mpGameScene->deleteGameItemGroups();

    mpGameLogic->stopGame();
}

int KBlocksDisplay::formIntFromByte(char *data)
{
    int value = 0;
    value += ((int)data[0]) & 0x000000FF;
    value += (((int)data[1]) <<  8) & 0x0000FF00;
    value += (((int)data[2]) << 16) & 0x00FF0000;
    value += (((int)data[3]) << 24) & 0xFF000000;
    return value;
}

void KBlocksDisplay::updateScore()
{
    mScore->setText(i18n("Score List : %1 - %2 - %3 - %4 - %5 - %6 - %7 - %8",
                         maScoreList[0], maScoreList[1], maScoreList[2], maScoreList[3],
                         maScoreList[4], maScoreList[5], maScoreList[6], maScoreList[7]));
}

void KBlocksDisplay::updateEvent()
{
    char tmpByteData[5] = {'|', 'r', 'g', '|', '\0'};
    mpNetClient->sendData(5, tmpByteData);
}

void KBlocksDisplay::updateGameDisplay(int size)
{
    char *tmpByteData = new char[size];

    int ret = mpNetClient->recvData(size, tmpByteData);
    if (ret < size) {
        return;
    }

    int gameID = tmpByteData[0];

    //int scorePoint = formIntFromByte(tmpByteData + 1);
    //int lineCount  = formIntFromByte(tmpByteData + 5);
    //int gameLevel  = formIntFromByte(tmpByteData + 9);
    maScoreList[gameID] = formIntFromByte(tmpByteData + 5);

    int tmpPieceCount = formIntFromByte(tmpByteData + 13);
    for (int i = 0; i < tmpPieceCount; ++i) {
        mpGameLogic->getSingleGame(gameID)->getPiece(i)->decodeData((unsigned char *)tmpByteData + 17 + i * 4);
    }

    formIntFromByte(tmpByteData + 17 + tmpPieceCount * 4);
    mpGameLogic->getSingleGame(gameID)->getField()->decodeData((unsigned char *)tmpByteData + 18 + tmpPieceCount * 4);

    updateScore();

    delete [] tmpByteData;
}
