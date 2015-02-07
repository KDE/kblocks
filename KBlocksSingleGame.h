/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSSINGLEGAME_H
#define KBLOCKSSINGLEGAME_H

#include <sys/time.h>

#include "SingleGameInterface.h"

#include "KBlocksField.h"
#include "KBlocksPiece.h"
#include "KBlocksPieceGenerator.h"
#include "KBlocksGameMessage.h"

#include "KBlocksGameRecorder.h"

#include "KBlocksDefine.h"

class KBlocksSingleGame : public SingleGameInterface
{
public:
    explicit KBlocksSingleGame(int gameIndex, int fieldWidth = 10, int fieldHeight = 20, int showPieceCount = 2, int messagePoolSize = 256);
    ~KBlocksSingleGame();

public:
    KBlocksField *getField();

    int getPieceCount();
    KBlocksPiece *getPiece(int index);

    bool isActive();
    bool isGameRunning();

    void setGameStandbyMode(bool flag);
    void setGameInterval(int interval);
    void setGameRecorder(KBlocksGameRecorder *p);

    int forceUpdateGame();
    int updateGame();
    int punishGame(int lineCount, int punishSeed);

    bool setCurrentPiece(int xPos, int yPos, int rotation);

    int startGame(int seed);
    int stopGame();

    int pauseGame(bool flag);
    int continueGame();

    bool pickGameResult(int *result);
    bool pickGameAction(int *type, int *action);

private:
    int doUpdateGame(bool force);
    bool runGameOneStep(int *gameResult);
    bool checkPieceTouchGround(KBlocksPiece *p);
    void freezePieceToField(KBlocksPiece *p);
    int removeFieldLines();
    void prepareNextPiece();

    timeLong getMillisecOfNow();

protected:
    KBlocksField *mpField;

    int mPieceCount;
    KBlocksPiece **mpPieceList;

private:
    KBlocksPieceGenerator *mpPieceGenerator;
    KBlocksGameMessage *mpGameMessage;
    KBlocksGameRecorder *mpGameRecorder;

    int mGameIndex;
    int mCurrentGameState;

    bool mStandbyMode;
    bool mStandbyFlag;
    int mGameInterval;
    timeLong mGameStartTime;
};

#endif

