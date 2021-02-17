/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2010-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/
#include "KBlocksScene.h"

#include "settings.h"

#include <QVarLengthArray>
#include <KLocalizedString>

#include "GraphicsInterface.h"
#include "SoundInterface.h"

KBlocksScene::KBlocksScene(
    GameLogicInterface *p,
    GraphicsInterface *graphics,
    SoundInterface *sound,
    int capacity
)
{
    mpGameLogic = p;
    mGameStarted = false;

    mSnapshotMode = false;
    mTopGameLevel = 0;
    mGroupCount = 0;
    mMaxCapacity = capacity;
    mSceneGamesPerLine = 4;
    mGameAnimEnabled = true;
    mWaitForAllUpdate = false;

    maGroupList = new KBlocksItemGroup*[capacity]();
    maGameScoreList = new KBlocksScore*[capacity]();
    maGameReadySignal = new bool[capacity]();

    mpGrafx = graphics;
    mpSnd = sound;

    int width = (capacity >= mSceneGamesPerLine) ? mSceneGamesPerLine : (capacity % mSceneGamesPerLine);
    int height = (int)(capacity / (mSceneGamesPerLine + 1)) + 1;
    setSceneRect(0, 0, mpGrafx->m_View_Size_Width * width,
                 mpGrafx->m_View_Size_Height * height);

    setItemIndexMethod(NoIndex);

    mUpdateInterval = 50;
    mUpdateTimer.setInterval(mUpdateInterval);
    connect(&mUpdateTimer, &QTimer::timeout, this, &KBlocksScene::updateGame);
    mUpdateTimer.stop();

    mMessageBox = nullptr;
}

KBlocksScene::~KBlocksScene()
{
    deleteGameItemGroups();
    delete [] maGameReadySignal;
    delete [] maGameScoreList;
    delete [] maGroupList;
}

KBlocksItemGroup *KBlocksScene::getItemGroup(int index)
{
    return maGroupList[index];
}

KBlocksScore *KBlocksScene::getScoreHandler(int index)
{
    return maGameScoreList[index];
}

void KBlocksScene::createGameItemGroups(int groupCount, bool snapshotMode)
{
    if (groupCount > mMaxCapacity) {
        mGroupCount = mMaxCapacity;
    }
    mGroupCount = groupCount;
    mSnapshotMode = snapshotMode;
    mTopGameLevel = 0;

    for (int i = 0; i < mGroupCount; i++) {
        maGroupList[i] = new KBlocksItemGroup(i, mpGameLogic->getSingleGame(i), mpGrafx, mpSnd, snapshotMode);
        maGroupList[i]->setUpdateInterval(mUpdateInterval);
        maGroupList[i]->setGameAnimEnabled(mGameAnimEnabled);
        maGroupList[i]->setWaitForAllUpdate(mWaitForAllUpdate);
        addItem(maGroupList[i]);

        maGameScoreList[i] = new KBlocksScore();
        maGameScoreList[i]->setLevelUpFactor(KBlocksScore_Level_x_Level_x_Factor, 1000);
        maGameScoreList[i]->setScoreUpFactor(10);

        maGameReadySignal[i] = false;
        connect(maGroupList[i], &KBlocksItemGroup::readyForAction, this, &KBlocksScene::readyForAction);
    }

    updateDimensions();

    //Our Message Item, hidden by default
    mMessageBox = new KGamePopupItem();
    mMessageBox->setMessageOpacity(0.9);
    addItem(mMessageBox);
}

void KBlocksScene::deleteGameItemGroups()
{
    if (mMessageBox) {
        removeItem(mMessageBox);
        delete mMessageBox;
        mMessageBox = nullptr;
    }

    for (int i = 0; i < mGroupCount; i++) {
        disconnect(maGroupList[i], &KBlocksItemGroup::readyForAction, this, &KBlocksScene::readyForAction);

        delete maGameScoreList[i];
        maGameScoreList[i] = 0;

        removeItem(maGroupList[i]);
        delete maGroupList[i];
        maGroupList[i] = 0;
    }
    mGroupCount = 0;
}

void KBlocksScene::setGamesPerLine(int count)
{
    mSceneGamesPerLine = count;
}

void KBlocksScene::setGameAnimEnabled(bool flag)
{
    mGameAnimEnabled = flag;
    for (int i = 0; i < mGroupCount; i++) {
        maGroupList[i]->setGameAnimEnabled(flag);
    }
}

void KBlocksScene::setWaitForAllUpdate(bool flag)
{
    mWaitForAllUpdate = flag;
    for (int i = 0; i < mGroupCount; i++) {
        maGroupList[i]->setWaitForAllUpdate(flag);
    }
}

void KBlocksScene::setUpdateInterval(int interval)
{
    mUpdateInterval = interval;
    mUpdateTimer.setInterval(mUpdateInterval);
    for (int i = 0; i < mGroupCount; i++) {
        maGroupList[i]->setUpdateInterval(mUpdateInterval);
    }
}

void KBlocksScene::setSoundsEnabled(bool enabled)
{
    mpSnd->setSoundsEnabled(enabled);
}

void KBlocksScene::readSettings(const QSize &viewSize)
{
    if (mpGrafx->theme()->fileName() != Settings::theme()) {
        mpGrafx->loadTheme(Settings::theme());
        mpGrafx->adjustForSize(viewSize);
        updateDimensions();
        // also update sounds fo rthe theme
        mpSnd->loadTheme(Settings::theme());
    }
}

void KBlocksScene::startGame()
{
    if (mGameStarted) {
        return;
    }
    mGameStarted = true;

    mTopGameLevel = 0;
    for (int i = 0; i < mGroupCount; i++) {
        maGroupList[i]->startGame();
    }

    if (!mSnapshotMode) {
        mUpdateTimer.start();
        QTimer::singleShot(500, this, &KBlocksScene::greetPlayer);
    }
}

void KBlocksScene::stopGame()
{
    if (!mGameStarted) {
        return;
    }
    mGameStarted = false;

    for (int i = 0; i < mGroupCount; i++) {
        maGroupList[i]->stopGame();
    }

    mUpdateTimer.stop();
}

void KBlocksScene::pauseGame(bool flag, bool fromUI)
{
    if (!mGameStarted) {
        return;
    }

    QString resuming(i18n("Game Resumed!"));
    QString pausing(i18n("Game Paused!"));

    for (int i = 0; i < mGroupCount; i++) {
        maGroupList[i]->pauseGame(flag);
    }

    if (!mSnapshotMode) {
        if (flag) {
            mUpdateTimer.stop();
            if (!fromUI) {
                showMessage(pausing, 2000);
            }
        } else {
            mUpdateTimer.start();
            if (!fromUI) {
                showMessage(resuming, 2000);
            }
        }
    }
}

void KBlocksScene::addScore(int gameIndex, int lineCount)
{
    if (!mSnapshotMode) {
        return;
    }
    maGameScoreList[gameIndex]->addScore(lineCount);
    emit scoreChanged(gameIndex, maGameScoreList[gameIndex]->getScorePoint(),
                      maGameScoreList[gameIndex]->getLineCount(),
                      maGameScoreList[gameIndex]->getGameLevel());
}

void KBlocksScene::updateDimensions()
{
    // TODO : Reset item position and scale
    int width = (mGroupCount >= mSceneGamesPerLine) ? mSceneGamesPerLine : (mGroupCount % mSceneGamesPerLine);
    int height = (int)(mGroupCount / (mSceneGamesPerLine + 1)) + 1;

    setSceneRect(0, 0, mpGrafx->m_View_Size_Width * width,
                 mpGrafx->m_View_Size_Height * height);

    for (int i = 0; i < mGroupCount; i++) {
        int left = mpGrafx->m_View_Size_Width * (i % mSceneGamesPerLine);
        int top = mpGrafx->m_View_Size_Height * ((int)(i / mSceneGamesPerLine));

        maGroupList[i]->setPos(left, top);
        maGroupList[i]->refreshPosition();
    }
}

void KBlocksScene::greetPlayer()
{
    QString greets(i18n("Game Start!"));
    showMessage(greets, 2000);
}

void KBlocksScene::gameOverPlayer()
{
    QString greets(i18n("Game Over!"));
    showMessage(greets, 2000);
}

void KBlocksScene::gameOverMultiWin()
{
    QString gameOver(i18n("You Win!"));
    showMessage(gameOver, 2000);
}

void KBlocksScene::gameOverMultiLose()
{
    QString gameOver(i18n("You Lose!"));
    showMessage(gameOver, 2000);
}

void KBlocksScene::showMessage(const QString &message, int ms)
{
    mMessageBox->setMessageTimeout(ms);
    mMessageBox->showMessage(message, KGamePopupItem::TopLeft);
}

void KBlocksScene::updateGame()
{
    if (mSnapshotMode) {
        return;
    }

    QVarLengthArray<int, 16> removedLines(mGroupCount);
    int gameCount = mpGameLogic->updateGame(removedLines.data());

    for (int i = 0; i < mGroupCount; i++) {
        if (removedLines[i] > 0) {
            if (maGameScoreList[i]->addScore(removedLines[i])) {
                int tmpLevel = maGameScoreList[i]->getGameLevel();
                if (mTopGameLevel < tmpLevel) {
                    mpGameLogic->levelUpGame(tmpLevel - mTopGameLevel);
                    mTopGameLevel = tmpLevel;
                }
            }
            emit scoreChanged(i, maGameScoreList[i]->getScorePoint(),
                              maGameScoreList[i]->getLineCount(),
                              maGameScoreList[i]->getGameLevel());
            // Play sound only for human player
            if (i == 0) {
                mpSnd->playSound(Sound::BlockRemove);
            }
        } else if (removedLines[i] == -1) {
            maGroupList[i]->stopGame();
            if (mGroupCount == 1) {
                QTimer::singleShot(500, this, &KBlocksScene::gameOverPlayer);
                emit isHighscore(0, maGameScoreList[0]->getScorePoint(),
                                 maGameScoreList[0]->getGameLevel());
            } else {
                if (i == 0) {
                    for (int j = 0; j < mGroupCount; j++) {
                        maGroupList[j]->stopGame();
                    }
                    QTimer::singleShot(500, this, &KBlocksScene::gameOverMultiLose);
                    emit isHighscore(0, maGameScoreList[0]->getScorePoint(),
                                     maGameScoreList[0]->getGameLevel());
                } else if (gameCount <= 1) {
                    maGroupList[0]->stopGame();
                    QTimer::singleShot(500, this, &KBlocksScene::gameOverMultiWin);
                    emit isHighscore(0, maGameScoreList[0]->getScorePoint(),
                                     maGameScoreList[0]->getGameLevel());
                }
            }
        }
    }
}

void KBlocksScene::readyForAction(int groupID)
{
    maGameReadySignal[groupID] = true;
    bool allReady = true;
    for (int i = 0; i < mGroupCount; i++) {
        if (!maGameReadySignal[i]) {
            allReady = false;
        }
    }
    if (allReady) {
        for (int i = 0; i < mGroupCount; i++) {
            if (mpGameLogic->getSingleGame(i)->isGameRunning()) {
                maGameReadySignal[i] = false;
            }
        }
        mpGameLogic->continueGame();
    }
}

void KBlocksScene::playMoveSound()
{
    mpSnd->playSound(Sound::BlockMove);
}

void KBlocksScene::playDropSound()
{
    mpSnd->playSound(Sound::BlockFall);
}

void KBlocksScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (mpGrafx->renderer()->isValid()) {
        mpGrafx->renderer()->render(painter, QStringLiteral("BACKGROUND"), rect);
    }
}

