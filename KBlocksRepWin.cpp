/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksRepWin.h"

#include <KLocale>
#include <KStatusBar>
#include <QPixmapCache>

#include <qdatetime.h>

KBlocksRepWin::KBlocksRepWin(const char * replayFile, bool binaryMode) : KMainWindow()
{
    //Use up to 3MB for global application pixmap cache
    QPixmapCache::setCacheLimit(3*1024);
    
    mpGameReplayer = new KBlocksGameReplayer(replayFile, binaryMode);
    
    mGameCount = mpGameReplayer->getGameCount();
    if (mGameCount == 0)
    {
        return;
    }
    
    mpGameLogic = new KBlocksGameLogic(mpGameReplayer);
    if (mpGameReplayer->isSameSeed())
    {
        mpGameLogic->setGameSeed(mpGameReplayer->getGameSeed());
    }
    else
    {
        mpGameLogic->setGameSeed(-mpGameReplayer->getGameSeed());
    }
    mpGameLogic->setGamePunish(false);
    mpGameLogic->setGameStandbyMode(false);
    mpGameLogic->setInitInterval(0);
    mpGameLogic->setLevelUpInterval(0);
    
    mpGameScene = new KBlocksScene(mpGameLogic, mGameCount);
    
    mpGameView = new KBlocksView(mpGameScene, this);
    mpGameView->show();
    setCentralWidget(mpGameView);
    
    mUpdateInterval = 1000;
    mUpdateTimer.setInterval(mUpdateInterval);
    connect(&mUpdateTimer, SIGNAL(timeout()), SLOT(replayOneStep()));
    mUpdateTimer.stop();
    
    mSnapshotFilename = QString("");
    mSnapshotFolder = QString("./snapshot/");
}

KBlocksRepWin::~KBlocksRepWin()
{
    delete mpGameView;
    delete mpGameScene;
    delete mpGameLogic;
    delete mpGameReplayer;
}

void KBlocksRepWin::setGamesPerLine(int count)
{
    mpGameScene->setGamesPerLine(count);
}

void KBlocksRepWin::setUpdateInterval(int interval)
{
    mUpdateInterval = interval;
    mUpdateTimer.setInterval(mUpdateInterval);
    mpGameScene->setUpdateInterval(interval);
}

void KBlocksRepWin::setReplayStepLength(int stepLen)
{
    mpGameReplayer->setStepLength(stepLen);
}

void KBlocksRepWin::setSnapshotFolder(const QString& folder)
{
    mSnapshotFolder = folder;
}

void KBlocksRepWin::setSnapshotFilename(const QString& fileName)
{
    mSnapshotFilename = fileName;
}

bool KBlocksRepWin::replayLoaded()
{
    return (mGameCount != 0);
}

void KBlocksRepWin::startReplay()
{
    mpGameLogic->startGame(mGameCount);
    
    mpGameScene->createGameItemGroups(mGameCount);
    mpGameScene->startGame();
    
    mUpdateTimer.start();
}

void KBlocksRepWin::stopReplay()
{
    mUpdateTimer.stop();
    
    mpGameScene->stopGame();
    mpGameScene->deleteGameItemGroups();
    
    mpGameLogic->stopGame();
}

QString KBlocksRepWin::getTimeString()
{
    QDate tmpDate = QDate::currentDate();
    QTime tmpTime = QTime::currentTime();
    QString result;
    result = QString("%1-%2-%3_%4-%5-%6_%7")
                    .arg(tmpDate.year(), 4, 10, QLatin1Char('0'))
                    .arg(tmpDate.month(), 2, 10, QLatin1Char('0'))
                    .arg(tmpDate.day(), 2, 10, QLatin1Char('0'))
                    .arg(tmpTime.hour(), 2, 10, QLatin1Char('0'))
                    .arg(tmpTime.minute(), 2, 10, QLatin1Char('0'))
                    .arg(tmpTime.second(), 2, 10, QLatin1Char('0'))
                    .arg(tmpTime.msec(), 3, 10, QLatin1Char('0'));
    return result;
}

void KBlocksRepWin::snapshotView()
{
    if (!mSnapshotFilename.isEmpty())
    {
        //mSnapshoter = QPixmap::grabWindow(mpGameView->winId());
        mSnapshoter = QPixmap::grabWidget(this);
        QString tmpFilename = mSnapshotFolder + mSnapshotFilename + QString("_")
                              + getTimeString() + QString(".png");
        mSnapshoter.save(tmpFilename);
    }
}

void KBlocksRepWin::replayOneStep()
{
    int tmpPieceChanged = 0;
    if (!mpGameLogic->playRecordOneStep(&tmpPieceChanged))
    {
        printf("Finished Replay!\n");
        mUpdateTimer.stop();
    }
    if (tmpPieceChanged != 0)
    {
        snapshotView();
    }
}
