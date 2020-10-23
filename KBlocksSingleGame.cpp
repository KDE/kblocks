/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksSingleGame.h"
#include "utils.h"
#include <QRandomGenerator>

KBlocksSingleGame::KBlocksSingleGame(int gameIndex, int fieldWidth, int fieldHeight, int showPieceCount, int messagePoolSize)
{
    mGameIndex = gameIndex;

    mpField = new KBlocksField(fieldWidth, fieldHeight);

    mPieceCount = showPieceCount;
    mpPieceList = new KBlocksPiece*[mPieceCount];
    for (int i = 0; i < mPieceCount; i++) {
        mpPieceList[i] = new KBlocksPiece();
    }

    mpPieceGenerator = new KBlocksPieceGenerator();
    mpGameMessage = new KBlocksGameMessage(messagePoolSize);
    mpGameRecorder = 0;

    mCurrentGameState = GameState_Stop;

    mStandbyMode = false;
    mStandbyFlag = false;

    mGameInterval = 0;
    mGameStartTime = 0;
}

KBlocksSingleGame::~KBlocksSingleGame()
{
    delete mpGameMessage;
    delete mpPieceGenerator;

    for (int i = 0; i < mPieceCount; i++) {
        delete mpPieceList[i];
    }
    delete [] mpPieceList;

    delete mpField;
}

KBlocksField *KBlocksSingleGame::getField()
{
    return mpField;
}

int KBlocksSingleGame::getPieceCount()
{
    return mPieceCount;
}

KBlocksPiece *KBlocksSingleGame::getPiece(int index)
{
    if ((index < 0) || (index >= mPieceCount)) {
        return 0;
    }
    return mpPieceList[index];
}

bool KBlocksSingleGame::isActive()
{
    if ((mCurrentGameState != GameState_Running) || mStandbyFlag) {
        return false;
    }
    return true;
}

bool KBlocksSingleGame::isGameRunning()
{
    if (mCurrentGameState == GameState_Stop) {
        return false;
    }
    return true;
}

void KBlocksSingleGame::setGameStandbyMode(bool flag)
{
    mStandbyMode = flag;
}

void KBlocksSingleGame::setGameInterval(int interval)
{
    mGameInterval = interval;
}

void KBlocksSingleGame::setGameRecorder(KBlocksGameRecorder *p)
{
    mpGameRecorder = p;
}

int KBlocksSingleGame::forceUpdateGame()
{
    return doUpdateGame(true);
}

int KBlocksSingleGame::updateGame()
{
    return doUpdateGame(false);
}

int KBlocksSingleGame::punishGame(int lineCount, int punishSeed)
{
    if (mCurrentGameState == GameState_Stop) {
        return GameResult_None;
    }

    int width = mpField->getWidth();

    int gameResult = GameResult_None;

    if (mpGameRecorder) {
        mpGameRecorder->append(mGameIndex, RecordDataType_PunishLineCount, lineCount);
        mpGameRecorder->append(mGameIndex, RecordDataType_PunishLineSeed, punishSeed);
    }

    int punishIndex = 0;
    auto random = QRandomGenerator::global();
    for (int i = 0; i < lineCount; i++) {
        setCurrentPiece(0, -1, 0);
        punishIndex = random->bounded(width);
        mpField->addPunishLine(lineCount, punishIndex);
    }

    if (lineCount > 0) {
        mpGameMessage->putGameAction(GameAction_Punish_Line, lineCount);
    }

    return gameResult;
}

bool KBlocksSingleGame::setCurrentPiece(int xPos, int yPos, int rotation)
{
    if ((mCurrentGameState != GameState_Running) || mStandbyFlag) {
        return false;
    }
    //FIXME: new without delete (is the new really necessary here?)
    QScopedPointer<KBlocksPiece> tmpPiece(new KBlocksPiece());

    tmpPiece->fromValue(mpPieceList[0]->toValue());
    tmpPiece->setPosX(mpPieceList[0]->getPosX() + xPos);
    if (mpPieceList[0]->getPosY() + yPos < 0) {
        tmpPiece->setPosY(0);
    } else {
        tmpPiece->setPosY(mpPieceList[0]->getPosY() + yPos);
    }
    tmpPiece->setRotation(mpPieceList[0]->getRotation() + rotation);

    if (checkPieceTouchGround(tmpPiece.data())) {
        return false;
    }

    mpPieceList[0]->setPosX(tmpPiece->getPosX());
    mpPieceList[0]->setPosY(tmpPiece->getPosY());
    mpPieceList[0]->setRotation(tmpPiece->getRotation());

    if (mpGameRecorder) {
        if (xPos < 0) {
            mpGameRecorder->append(mGameIndex, RecordDataType_MovePieceLeft, 1);
        }
        if (xPos > 0) {
            mpGameRecorder->append(mGameIndex, RecordDataType_MovePieceRight, 1);
        }
        if (yPos < 0) {
            mpGameRecorder->append(mGameIndex, RecordDataType_MovePieceUp, 1);
        }
        if (yPos > 0) {
            mpGameRecorder->append(mGameIndex, RecordDataType_MovePieceDown, 1);
        }
        if (rotation < 0) {
            mpGameRecorder->append(mGameIndex, RecordDataType_RotatePieceCW, 1);
        }
        if (rotation > 0) {
            mpGameRecorder->append(mGameIndex, RecordDataType_RotatePieceCCW, 1);
        }
    }

    return true;
}

int KBlocksSingleGame::startGame(int seed)
{
    if (mCurrentGameState != GameState_Stop) {
        return mCurrentGameState;
    }

    mpPieceGenerator->genList(seed);

    for (int i = 0; i < mPieceCount; i++) {
        mpPieceList[i]->fromValue(mpPieceGenerator->getPiece());
    }

    mpPieceList[0]->setPosX(mpField->getWidth() / 2);
    mpPieceList[0]->setPosY(0);

    mpPieceList[1]->setPosX(2);
    mpPieceList[1]->setPosY(2);

    mpGameMessage->clearGameResult();
    mpGameMessage->clearGameAction();

    mCurrentGameState = GameState_Running;

    mGameStartTime = Utils::getMillisecOfNow();

    return mCurrentGameState;
}

int KBlocksSingleGame::stopGame()
{
    if (mCurrentGameState != GameState_Stop) {
        mCurrentGameState = GameState_Stop;
        emit gameStopped();
    }

    return mCurrentGameState;
}

int KBlocksSingleGame::pauseGame(bool flag)
{
    if ((mCurrentGameState == GameState_Running) && flag) {
        mCurrentGameState = GameState_Pause;
    } else if ((mCurrentGameState == GameState_Pause) && (!flag)) {
        mCurrentGameState = GameState_Running;

        mGameStartTime = Utils::getMillisecOfNow();
    }

    return mCurrentGameState;
}

int KBlocksSingleGame::continueGame()
{
    if ((mCurrentGameState != GameState_Stop) && mStandbyFlag) {
        mStandbyFlag = false;

        mGameStartTime = Utils::getMillisecOfNow();
    }

    return mCurrentGameState;
}

bool KBlocksSingleGame::pickGameResult(int *result)
{
    return mpGameMessage->pickGameResult(result);
}

bool KBlocksSingleGame::pickGameAction(int *type, int *action)
{
    return mpGameMessage->pickGameAction(type, action);
}

int KBlocksSingleGame::doUpdateGame(bool force)
{
    if (mCurrentGameState == GameState_Stop) {
        return GameResult_Game_Over;
    } else if ((mCurrentGameState != GameState_Running) || mStandbyFlag) {
        return GameResult_None;
    }

    timeLong tmpCurTime = Utils::getMillisecOfNow();

    int gameResult = GameResult_None;

    if (force) {
        runGameOneStep(&gameResult);
    } else {
        if (mGameInterval < 0) {
            return gameResult;
        }

        while (1) {
            if (mGameStartTime + mGameInterval > tmpCurTime) {
                break;
            }

            mGameStartTime += mGameInterval;

            if (runGameOneStep(&gameResult) || (mGameInterval == 0)) {
                break;
            }
        }
    }

    return gameResult;
}

bool KBlocksSingleGame::runGameOneStep(int *gameResult)
{
    if (!setCurrentPiece(0, 1, 0)) {
        *gameResult = GameResult_Next_Piece;

        freezePieceToField(mpPieceList[0]);

        *gameResult += removeFieldLines();

        if (mStandbyMode) {
            mStandbyFlag = true;
        }

        prepareNextPiece();
        if (checkPieceTouchGround(mpPieceList[0])) {
            *gameResult = GameResult_Game_Over;
            mpGameMessage->putGameResult(-1);
            stopGame();
        }

        if (mpGameRecorder) {
            mpGameRecorder->append(mGameIndex, RecordDataType_GameOneStep, 1);
        }

        return true;
    } else {
        *gameResult = GameResult_One_Step;

        return false;
    }
}

bool KBlocksSingleGame::checkPieceTouchGround(KBlocksPiece *p)
{
    for (int i = 0; i < 4; i++) {
        int posX = p->getCellPosX(i);
        int posY = p->getCellPosY(i);
        if (mpField->getCell(posX, posY)) {
            return true;
        }
    }
    return false;
}

void KBlocksSingleGame::freezePieceToField(KBlocksPiece *p)
{
    mpGameMessage->putGameAction(GameAction_Freeze_Piece_Color, mpPieceList[0]->getType());
    for (int i = 0; i < 4; i++) {
        int posX = p->getCellPosX(i);
        int posY = p->getCellPosY(i);
        mpField->setCell(posX, posY, true);
        mpGameMessage->putGameAction(GameAction_Freeze_Piece_X, posX);
        mpGameMessage->putGameAction(GameAction_Freeze_Piece_Y, posY);
    }
}

int KBlocksSingleGame::removeFieldLines()
{
    int lineCount = 0;

    int maxLines = mpField->getHeight();
    for (int i = 0; i < maxLines; i++) {
        if (mpField->checkFilledLine(i)) {
            mpGameMessage->putGameAction(GameAction_Remove_Line, i);
            mpField->removeFilledLine(i);
            lineCount++;
        }
    }

    if (lineCount > 0) {
        mpGameMessage->putGameResult(lineCount);
    }

    return lineCount;
}

void KBlocksSingleGame::prepareNextPiece()
{
    for (int i = 0; i < mPieceCount - 1; i++) {
        mpPieceList[i]->fromValue(mpPieceList[i + 1]->toValue());
    }

    int pieceValue = mpPieceGenerator->getPiece();
    mpPieceList[mPieceCount - 1]->fromValue(pieceValue);

    mpPieceList[0]->setPosX(mpField->getWidth() / 2);
    mpPieceList[0]->setPosY(0);

    mpPieceList[1]->setPosX(2);
    mpPieceList[1]->setPosY(2);

    for (int i = 0; i < 4; i++) {
        int posX = mpPieceList[0]->getCellPosX(i);
        int posY = mpPieceList[0]->getCellPosY(i);
        mpGameMessage->putGameAction(GameAction_New_Piece_X, posX);
        mpGameMessage->putGameAction(GameAction_New_Piece_Y, posY);
    }
}

