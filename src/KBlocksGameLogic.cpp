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
#include "KBlocksGameLogic.h"
#include <QRandomGenerator>


KBlocksGameLogic::KBlocksGameLogic(int capacity, bool record)
{
    mGameCount = 0;
    mGameMax = capacity;

    mGameSeed = 0;
    mPunishFlag = true;

    mStandbyMode = false;
    mGameInterval = 0;

    mInitialInterval = 0;
    mLevelUpInterval = 0;

    maGameList = new KBlocksSingleGame*[capacity];

    if (record) {
        mpGameRecorder = new KBlocksGameRecorder();
    } else {
        mpGameRecorder = nullptr;
    }
    mpGameReplayer = 0;
}

KBlocksGameLogic::KBlocksGameLogic(KBlocksGameReplayer *p)
{
    mGameCount = 0;
    mGameMax = p->getGameCount();

    mGameSeed = 0;
    mPunishFlag = true;

    mStandbyMode = false;
    mGameInterval = 0;

    mInitialInterval = 0;
    mLevelUpInterval = 0;

    maGameList = new KBlocksSingleGame*[mGameMax];

    mpGameRecorder = 0;
    mpGameReplayer = p;
}

KBlocksGameLogic::~KBlocksGameLogic()
{
    if (mpGameRecorder) {
        delete mpGameRecorder;
    }
    delete [] maGameList;
}

int KBlocksGameLogic::getActiveGameCount()
{
    int result = 0;
    for (int i = 0; i < mGameCount; i++) {
        if (maGameList[i]->isGameRunning()) {
            result++;
        }
    }
    return result;
}

KBlocksSingleGame *KBlocksGameLogic::getSingleGame(int index)
{
    if ((index < 0) || (index >= mGameCount)) {
        return 0;
    }
    return maGameList[index];
}

bool KBlocksGameLogic::playRecordOneStep(int *changedPiece)
{
    int tmpCount;
    vector<KBlocksReplayData> tmpData;
    *changedPiece = 0;
    if (mpGameReplayer->getNextRecords(&tmpData)) {
        for (size_t i = 0; i < tmpData.size(); i++) {
            switch (tmpData[i].type) {
            case RecordDataType_MovePieceLeft:
                maGameList[tmpData[i].index]->setCurrentPiece(-1, 0, 0);
//                    printf("[%d] - Move Left\n", tmpData[i].index);
                break;
            case RecordDataType_MovePieceRight:
                maGameList[tmpData[i].index]->setCurrentPiece(1, 0, 0);
//                    printf("[%d] - Move Right\n", tmpData[i].index);
                break;
            case RecordDataType_MovePieceUp:
                maGameList[tmpData[i].index]->setCurrentPiece(0, -1, 0);
//                    printf("[%d] - Move Up\n", tmpData[i].index);
                break;
            case RecordDataType_MovePieceDown:
                maGameList[tmpData[i].index]->setCurrentPiece(0, 1, 0);
//                    printf("[%d] - Move Down\n", tmpData[i].index);
                break;
            case RecordDataType_RotatePieceCW:
                maGameList[tmpData[i].index]->setCurrentPiece(0, 0, -1);
//                    printf("[%d] - Rotate Left\n", tmpData[i].index);
                break;
            case RecordDataType_RotatePieceCCW:
                maGameList[tmpData[i].index]->setCurrentPiece(0, 0, 1);
//                    printf("[%d] - Rotate Right\n", tmpData[i].index);
                break;
            case RecordDataType_GameOneStep:
                maGameList[tmpData[i].index]->forceUpdateGame();
                *changedPiece |= (1 << tmpData[i].index);
//                    printf("[%d] - Game Stepped\n", tmpData[i].index);
                break;
            case RecordDataType_PunishLineCount:
                tmpCount = tmpData[i].value;
                ++i;
                if (tmpData[i].type == RecordDataType_PunishLineSeed) {
                    maGameList[tmpData[i].index]->punishGame(tmpCount, tmpData[i].value);
//                        printf("[%d] - Punish %d(%d)\n", tmpData[i].index, tmpCount, tmpData[i].value);
                }
                break;
            case RecordDataType_Skipped:
//                    printf("[%d] - A skipper step\n", tmpData[i].index);
                break;
            default:
//                    printf("[%d] - Error default\n", tmpData[i].index);
                break;
            }
        }
        return true;
    } else {
        return false;
    }
}

void KBlocksGameLogic::saveRecord(const char *fileName, bool binaryMode)
{
    if (mpGameRecorder) {
        mpGameRecorder->save(fileName, binaryMode);
    }
}

int KBlocksGameLogic::levelUpGame(int level)
{
    mGameInterval -= (mLevelUpInterval * level);
    if (mGameInterval < (mLevelUpInterval * 2)) {
        mGameInterval = mLevelUpInterval;
    }
    setGameInterval(mGameInterval);
    return mGameInterval;
}

int KBlocksGameLogic::updateGame(int *lineList)
{
    for (int i = 0; i < mGameCount; i++) {
        int tmpTotal = 0;
        int tmpValue = 0;
        int tmpPunishCount = 0;

        while (maGameList[i]->pickGameResult(&tmpValue)) {
            if (tmpValue < 0) {
                tmpTotal = -1;
                tmpPunishCount = 0;
                break;
            }
            tmpTotal += tmpValue;
            tmpPunishCount += (tmpValue - 1);
        }

        lineList[i] = tmpTotal;

        if ((mPunishFlag) && (tmpPunishCount > 0)) {
            int punishSeed = QRandomGenerator::global()->bounded(256);
            for (int j = 0; j < i; j++) {
                maGameList[j]->punishGame(tmpPunishCount, punishSeed);
            }
            for (int j = i + 1; j < mGameCount; j++) {
                maGameList[j]->punishGame(tmpPunishCount, punishSeed);
            }
        }
    }

    return getActiveGameCount();
}

void KBlocksGameLogic::setGameSeed(int seed)
{
    mGameSeed = (seed % 256);
}

void KBlocksGameLogic::setGamePunish(bool flag)
{
    mPunishFlag = flag;
}

void KBlocksGameLogic::setGameStandbyMode(bool flag)
{
    mStandbyMode = flag;
    for (int i = 0; i < mGameCount; i++) {
        maGameList[i]->setGameStandbyMode(mStandbyMode);
    }
}

void KBlocksGameLogic::setGameInterval(int interval)
{
    mGameInterval = interval;
    for (int i = 0; i < mGameCount; i++) {
        maGameList[i]->setGameInterval(mGameInterval);
    }
}

void KBlocksGameLogic::setInitInterval(int interval)
{
    mInitialInterval = interval;
}

void KBlocksGameLogic::setLevelUpInterval(int interval)
{
    mLevelUpInterval = interval;
}

bool KBlocksGameLogic::startGame(int gameCount)
{
    if (mGameCount != 0) {
        return false;
    }
    mGameInterval = mInitialInterval;
    createSingleGames(gameCount);
    return true;
}

bool KBlocksGameLogic::stopGame()
{
    if (mGameCount == 0) {
        return false;
    }

    for (int i = 0; i < mGameCount; ++i) {
        maGameList[i]->stopGame();
    }
    emit allGamesStopped();

    return true;
}

void KBlocksGameLogic::pauseGame(bool pauseFlag)
{
    for (int i = 0; i < mGameCount; i++) {
        maGameList[i]->pauseGame(pauseFlag);
    }
}

void KBlocksGameLogic::continueGame()
{
    for (int i = 0; i < mGameCount; i++) {
        maGameList[i]->continueGame();
    }
}

bool KBlocksGameLogic::hasSingleGames()
{
    return mGameCount != 0;
}

void KBlocksGameLogic::createSingleGames(int gameCount)
{
    if (gameCount > mGameMax) {
        gameCount = mGameMax;
    }
    mGameCount = gameCount;

    if (mpGameRecorder) {
        mpGameRecorder->append(0, RecordDataType_GameCount, mGameCount);
        if (mGameSeed < 0) {
            mpGameRecorder->append(0, RecordDataType_GameSeed, -mGameSeed);
        } else {
            mpGameRecorder->append(1, RecordDataType_GameSeed, mGameSeed);
        }
    }

    int *seedList = new int[mGameCount];
    if (mGameSeed < 0) {
        mGameSeed = -mGameSeed;
        for (int i = 0; i < mGameCount; i++) {
            seedList[i] = QRandomGenerator::global()->bounded(256);
        }
    } else {
        for (int i = 0; i < mGameCount; i++) {
            seedList[i] = mGameSeed;
        }
    }

    for (int i = 0; i < mGameCount; i++) {
        maGameList[i] = new KBlocksSingleGame(i);
        connect(maGameList[i], &KBlocksSingleGame::gameStopped,
                 this, &KBlocksGameLogic::stopGame);
        maGameList[i]->setGameStandbyMode(mStandbyMode);
        maGameList[i]->setGameInterval(mGameInterval);
        maGameList[i]->setGameRecorder(mpGameRecorder);
        maGameList[i]->startGame(seedList[i]);
    }

    delete[] seedList;
}

void KBlocksGameLogic::deleteSingleGames()
{
    for (int i = 0; i < mGameCount; i++) {
        maGameList[i]->stopGame();
        delete maGameList[i];
        maGameList[i] = 0;
    }
    mGameCount = 0;
}
