/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSSINGLEGAME_H
#define KBLOCKSSINGLEGAME_H

#include <QObject>

#include "SingleGameInterface.h"

#include "KBlocksField.h"
#include "KBlocksPiece.h"
#include "KBlocksPieceGenerator.h"
#include "KBlocksGameMessage.h"

#include "KBlocksGameRecorder.h"

#include "KBlocksDefine.h"

class KBlocksSingleGame : public QObject, public SingleGameInterface
{
    Q_OBJECT

public:
    explicit KBlocksSingleGame(int gameIndex, int fieldWidth = 10, int fieldHeight = 20, int showPieceCount = 2, int messagePoolSize = 256);
    ~KBlocksSingleGame();

public:
    KBlocksField *getField() override;

    int getPieceCount() override;
    KBlocksPiece *getPiece(int index) override;

    bool isActive() override;
    bool isGameRunning() override;

    void setGameStandbyMode(bool flag);
    void setGameInterval(int interval);
    void setGameRecorder(KBlocksGameRecorder *p);

    int forceUpdateGame() override;
    int updateGame() override;
    int punishGame(int lineCount, int punishSeed);

    bool setCurrentPiece(int xPos, int yPos, int rotation) override;

    int startGame(int seed);
    int stopGame();

    int pauseGame(bool flag);
    int continueGame() override;

    bool pickGameResult(int *result) override;
    bool pickGameAction(int *type, int *action) override;

signals:
    void gameStopped();

private:
    int doUpdateGame(bool force);
    bool runGameOneStep(int *gameResult);
    bool checkPieceTouchGround(KBlocksPiece *p);
    void freezePieceToField(KBlocksPiece *p);
    int removeFieldLines();
    void prepareNextPiece();

protected:
    KBlocksField *mpField = nullptr;

    int mPieceCount;
    KBlocksPiece **mpPieceList = nullptr;

private:
    KBlocksPieceGenerator *mpPieceGenerator = nullptr;
    KBlocksGameMessage *mpGameMessage = nullptr;
    KBlocksGameRecorder *mpGameRecorder = nullptr;

    int mGameIndex;
    int mCurrentGameState;

    bool mStandbyMode;
    bool mStandbyFlag;
    int mGameInterval;
    timeLong mGameStartTime;
};

#endif

