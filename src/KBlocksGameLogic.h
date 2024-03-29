/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#ifndef KBLOCKSGAMELOGIC_H
#define KBLOCKSGAMELOGIC_H

#include <stdlib.h>

#include "GameLogicInterface.h"
#include "SingleGameInterface.h"

#include "KBlocksDefine.h"

#include "KBlocksGameRecorder.h"
#include "KBlocksGameReplayer.h"

#include "KBlocksSingleGame.h"

class KBlocksGameLogic : public GameLogicInterface
{
    Q_OBJECT

public:
    explicit KBlocksGameLogic(int capacity, bool record = false);
    explicit KBlocksGameLogic(KBlocksGameReplayer *p);
    ~KBlocksGameLogic() override;

public:
    int getActiveGameCount();
    KBlocksSingleGame *getSingleGame(int index) override;

    bool playRecordOneStep(int *changedPiece);
    void saveRecord(const char *fileName, bool binaryMode = true);

    int levelUpGame(int level) override;
    int updateGame(int *lineList) override;

    void setGameSeed(int seed) override;
    void setGamePunish(bool flag);

    void setGameStandbyMode(bool flag) override;
    void setGameInterval(int interval);
    void setInitInterval(int interval);
    void setLevelUpInterval(int interval);

    bool startGame(int gameCount) override;

    void pauseGame(bool pauseFlag) override;
    void continueGame() override;

    /**
     * Return whether single games have been created.
     */
    bool hasSingleGames() override;
    void deleteSingleGames() override;

public Q_SLOTS:
    bool stopGame() override;

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

