/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSGAMELOGIC_H
#define KBLOCKSGAMELOGIC_H

#include <stdlib.h>
#include <QObject>

#include "GameLogicInterface.h"
#include "SingleGameInterface.h"

#include "KBlocksDefine.h"

#include "KBlocksGameRecorder.h"
#include "KBlocksGameReplayer.h"

#include "KBlocksSingleGame.h"

class KBlocksGameLogic : public QObject, public GameLogicInterface
{
    Q_OBJECT

public:
    explicit KBlocksGameLogic(int capacity, bool record = false);
    explicit KBlocksGameLogic(KBlocksGameReplayer *p);
    ~KBlocksGameLogic();

public:
    int getActiveGameCount();
    KBlocksSingleGame *getSingleGame(int index) override;

    bool playRecordOneStep(int *changedPiece);
    void saveRecord(const char *fileName, bool binaryMode = true);

    int levelUpGame(int level) override;
    int updateGame(int *lineList) override;

    void setGameSeed(int seed);
    void setGamePunish(bool flag);

    void setGameStandbyMode(bool flag);
    void setGameInterval(int interval);
    void setInitInterval(int interval);
    void setLevelUpInterval(int interval);

    bool startGame(int gameCount) override;

    void pauseGame(bool pauseFlag) override;
    void continueGame() override;

    bool deleteSingleGames();

public slots:
    bool stopGame() override;

signals:
    void allGamesStopped();

private:
    void createSingleGames(int gameCount);

protected:
    KBlocksSingleGame **maGameList;

private:
    int mGameMax;
    int mGameCount;

    int mGameSeed;
    int mPunishFlag;

    bool mStandbyMode;
    int mGameInterval;
    int mInitialInterval;
    int mLevelUpInterval;

    KBlocksGameRecorder *mpGameRecorder = nullptr;
    KBlocksGameReplayer *mpGameReplayer = nullptr;
};

#endif

