/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksDisplay.h"

#include "AI/KBlocksAILog.h"

#include <KLocale>
#include <KStatusBar>
#include <QPixmapCache>

KBlocksDisplay::KBlocksDisplay(int gameCount, const string& serverIP, int localPort) : KMainWindow()
{
    //Use up to 3MB for global application pixmap cache
    QPixmapCache::setCacheLimit(3*1024);
    
    mpNetClient = new KBlocksNetClient(serverIP, localPort);
    mpNetClient->setTimeOut(1000000);
    
    mGameCount = gameCount;
    mpGameLogic = new KBlocksGameLogic(mGameCount);
    mpGameLogic->setGameSeed(0);
    mpGameLogic->setGamePunish(false);
    mpGameLogic->setGameStandbyMode(true);
    mpGameLogic->setInitInterval(0);
    mpGameLogic->setLevelUpInterval(0);
    
    mpGameScene = new KBlocksScene(mpGameLogic, mGameCount);
    mpGameScene->setGameAnimEnabled(false);
    mpGameScene->setWaitForAllUpdate(false);
    
    mpGameView = new KBlocksView(mpGameScene, this);
	mpGameView->show();
    setCentralWidget(mpGameView);
    
    mUpdateInterval = 1000;
    mUpdateTimer.setInterval(mUpdateInterval);
    connect(&mUpdateTimer, SIGNAL(timeout()), SLOT(updateGameDisplay()));
    mUpdateTimer.stop();
    
    statusBar()->insertItem( i18n("Score List : 0 - 0 - 0 - 0 - 0 - 0 - 0 - 0"), 0 );
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
    for(int i = 0; i < mGameCount; i++)
    {
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

int KBlocksDisplay::formIntFromByte(unsigned char * data)
{
    int value = 0;
    value += ((int)data[0]) & 0x000000FF;
    value += (((int)data[1]) <<  8) & 0x0000FF00;
    value += (((int)data[2]) << 16) & 0x00FF0000;
    value += (((int)data[3]) << 24) & 0xFF000000;
    return value;
}

void KBlocksDisplay::updateGameDisplay()
{
    int tmpByteCount;
    unsigned char* tmpByteData = new unsigned char[256];
    
    tmpByteData[0] = '|';
    tmpByteData[1] = 'r';
    tmpByteData[2] = 'g';
    tmpByteData[3] = '|';
    tmpByteData[4] = '\0';
    
    if (mpNetClient->sendData(5, tmpByteData) > 0)
    {
        int scoreList[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        for(int i = 0; i < mGameCount; i++)
        {
            int ret = mpNetClient->recvData(256, tmpByteData);
            if (ret < 0)
            {
                return;
            }
            
            int gameID = tmpByteData[0];
            
            //int scorePoint = formIntFromByte(tmpByteData + 1);
            //int lineCount  = formIntFromByte(tmpByteData + 5);
            //int gameLevel  = formIntFromByte(tmpByteData + 9);
            scoreList[gameID] = formIntFromByte(tmpByteData + 5);
            
            mpGameLogic->getSingleGame(gameID)->getPiece(0)->decodeData(tmpByteData + 13);
            mpGameLogic->getSingleGame(gameID)->getPiece(1)->decodeData(tmpByteData + 17);
            
            tmpByteCount = tmpByteData[21];
            mpGameLogic->getSingleGame(gameID)->getField()->decodeData(tmpByteData + 22);
        }
        
        statusBar()->changeItem( i18n("Score List : %1 - %2 - %3 - %4 - %5 - %6 - %7 - %8",
                                      scoreList[0], scoreList[1], scoreList[2], scoreList[3], 
                                      scoreList[4], scoreList[5], scoreList[6], scoreList[7]), 0 );
    }
    
    delete [] tmpByteData;
}
