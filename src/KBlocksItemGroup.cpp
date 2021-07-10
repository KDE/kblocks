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
#include "GraphicsInterface.h"
#include "SoundInterface.h"

#include "KBlocksItemGroup.h"
#include "KBlocksSvgItem.h"
#include "SvgItemInterface.h"

KBlocksItemGroup::KBlocksItemGroup(int groupID, SingleGameInterface *p, GraphicsInterface *pG, SoundInterface *pS, bool snapshotMode)
{
    mGroupID = groupID;
    mpSingleGame = p;
    mpGrafx = pG;
    mpSnd = pS;

    updateGraphicInfo();

    mpGameLayout = new KBlocksLayout(mpSingleGame->getField(), mpSingleGame->getPiece(0), mpSingleGame->getPiece(1));

    mpBackground = new KBlocksSvgItem(mpGameLayout, -1, 0, 0);
    mpBackground->setSharedRenderer(mpGrafx->renderer());
    mpBackground->setElementId(QStringLiteral("VIEW"));
    addToGroup(mpBackground);

    mMaxPrepareCellNum = PREPARE_AREA_WIDTH * PREPARE_AREA_WIDTH;
    maPrepareCells = new SvgItemInterface*[mMaxPrepareCellNum];
    for (int i = 0; i < mMaxPrepareCellNum; i++) {
        maPrepareCells[i] = new KBlocksSvgItem(mpGameLayout, KBlocksSvgItem_PrepareArea,
                                               i % PREPARE_AREA_WIDTH, i / PREPARE_AREA_WIDTH);
        maPrepareCells[i]->setSharedRenderer(mpGrafx->renderer());
        maPrepareCells[i]->setElementId(QStringLiteral("BLOCK_0"));
        maPrepareCells[i]->setVisible(false);
        addToGroup(maPrepareCells[i]);
    }

    mMaxFreezeCellNum = (mFieldWidth * mFieldHeight);
    maFreezeCells = new SvgItemInterface*[mMaxFreezeCellNum];
    for (int i = 0; i < mMaxFreezeCellNum; i++) {
        maFreezeCells[i] = new KBlocksSvgItem(mpGameLayout, KBlocksSvgItem_FieldArea,
                                              i % mFieldWidth, i / mFieldWidth);
        maFreezeCells[i]->setSharedRenderer(mpGrafx->renderer());
        maFreezeCells[i]->setElementId(QStringLiteral("BLOCK_0"));
        maFreezeCells[i]->setVisible(false);
        addToGroup(maFreezeCells[i]);
    }

    mGameAnimEnabled = true;
    mWaitForAllUpdate = false;
    mpAnimator = new KBlocksAnimator();
    connect(mpAnimator, &KBlocksAnimator::animFinished, this, &KBlocksItemGroup::endAnimation);

    mFadeInItems.clear();
    mFadeOutItems.clear();
    mDropItems.clear();

    mUpdateInterval = 50;
    mUpdateTimer.setInterval(mUpdateInterval);
    if (snapshotMode) {
        connect(&mUpdateTimer, &QTimer::timeout, this, &KBlocksItemGroup::updateSnapshot);
    } else {
        connect(&mUpdateTimer, &QTimer::timeout, this, &KBlocksItemGroup::updateGame);
    }
    mUpdateTimer.stop();
}

KBlocksItemGroup::~KBlocksItemGroup()
{
    delete mpAnimator;

    for (int i = 0; i < mMaxFreezeCellNum; i++) {
        removeFromGroup(maFreezeCells[i]);
        delete maFreezeCells[i];
    }
    delete [] maFreezeCells;

    for (int i = 0; i < mMaxPrepareCellNum; i++) {
        removeFromGroup(maPrepareCells[i]);
        delete maPrepareCells[i];
    }
    delete [] maPrepareCells;

    removeFromGroup(mpBackground);
    delete mpBackground;

    delete mpGameLayout;
}

void KBlocksItemGroup::setUpdateInterval(int interval)
{
    mUpdateInterval = interval;
    mUpdateTimer.setInterval(mUpdateInterval);
}

void KBlocksItemGroup::setGameAnimEnabled(bool flag)
{
    mGameAnimEnabled = flag;
}

void KBlocksItemGroup::setWaitForAllUpdate(bool flag)
{
    mWaitForAllUpdate = flag;
}

void KBlocksItemGroup::refreshPosition()
{
    updateGraphicInfo();

    mpBackground->setElementId(QStringLiteral("VIEW"));
    mpBackground->setPos(0, 0);

    for (int i = 0; i < mMaxPrepareCellNum; i++) {
        maPrepareCells[i]->setPos(mPrepareLeft + mItemSize * (i % PREPARE_AREA_WIDTH),
                                  mPrepareTop + mItemSize * (i / PREPARE_AREA_WIDTH));
        maPrepareCells[i]->clearCache();
    }

    for (int i = 0; i < mMaxFreezeCellNum; i++) {
        maFreezeCells[i]->setPos(mFieldLeft + mItemSize * (i % mFieldWidth),
                                 mFieldTop + mItemSize * (i / mFieldWidth));
        maFreezeCells[i]->clearCache();
    }
}

void KBlocksItemGroup::startGame()
{
    mUpdateTimer.start();
}

void KBlocksItemGroup::stopGame()
{
    updateGame();
    mUpdateTimer.stop();
}

void KBlocksItemGroup::pauseGame(bool flag)
{
    if (flag) {
        mUpdateTimer.stop();
    } else {
        mUpdateTimer.start();
    }
}

void KBlocksItemGroup::updateGame()
{
    int gameResult = mpSingleGame->updateGame();

    bool hasRemovedLines = updateLayout();

    if (gameResult == GameResult_Game_Over) {
        refreshItems();
        mUpdateTimer.stop();
        return;
    }

    if (hasRemovedLines && mGameAnimEnabled) {
        mUpdateTimer.stop();
        fadeOutOldLine();
        dropFreezeLine();
    } else {
        refreshItems();
        if (hasRemovedLines) {
            if (!mWaitForAllUpdate) {
                mpSingleGame->continueGame();
            } else {
                emit readyForAction(mGroupID);
            }
        }
    }
}

void KBlocksItemGroup::updateSnapshot()
{
    mpGameLayout->updateSnapshot();
    refreshItems();
}

void KBlocksItemGroup::endAnimation(int animType)
{
    switch (animType) {
    case KBlocks_Animation_Fade_In:
        mpAnimator->deleteFadeAnim();
        if (!mWaitForAllUpdate) {
            mpSingleGame->continueGame();
        } else {
            emit readyForAction(mGroupID);
        }
        mUpdateTimer.start();
        break;
    case KBlocks_Animation_Fade_Out:
        mpAnimator->deleteFadeAnim();
        fadeInNewPiece();
        break;
    case KBlocks_Animation_Drop:
        mpAnimator->deleteDropAnim();
        break;
    default:
        break;
    }
}

bool KBlocksItemGroup::updateLayout()
{
    int tmpActionType = GameAction_None;
    int tmpActionData = 0;
    QList<int> tmpDataList;

    bool hasAnim = false;
    int pieceCellCount = mpSingleGame->getPiece(0)->getCellCount() * 2;

    mpGameLayout->beginUpdate(&tmpDataList);
    refreshItemByPos(tmpDataList);
    tmpDataList.clear();

    mRemovedLine.clear();
    mPunishLine.clear();
    mNewPiecePos.clear();

    while (mpSingleGame->pickGameAction(&tmpActionType, &tmpActionData)) {
        switch (tmpActionType) {
        case GameAction_Freeze_Piece_Color:
            tmpDataList.append(tmpActionData);
            for (int i = 0; i < pieceCellCount; i++) {
                tmpActionType = GameAction_None;
                mpSingleGame->pickGameAction(&tmpActionType, &tmpActionData);
                tmpDataList.append(tmpActionData);
            }
            mpGameLayout->updateLayout(KBlocksLayout_Update_FreezePiece, tmpDataList);
            tmpDataList.takeFirst();
            refreshItemByPos(tmpDataList);
            break;
        case GameAction_Remove_Line:
            mRemovedLine.append(tmpActionData);
            tmpDataList.append(tmpActionData);
            mpGameLayout->updateLayout(KBlocksLayout_Update_RemoveLine, tmpDataList);
            hasAnim = true;
            break;
        case GameAction_Punish_Line:
            mPunishLine.append(tmpActionData);
            tmpDataList.append(tmpActionData);
            mpGameLayout->updateLayout(KBlocksLayout_Update_PunishLine, tmpDataList);
            break;
        case GameAction_New_Piece_X:
        case GameAction_New_Piece_Y:
            mNewPiecePos.append(tmpActionData);
            hasAnim = true;
            break;
        }
        tmpActionType = GameAction_None;
        tmpActionData = 0;
        tmpDataList.clear();
    }

    mpGameLayout->endUpdate();

    return hasAnim;
}

void KBlocksItemGroup::refreshItems()
{
    for (int i = 0; i < mMaxFreezeCellNum; i++) {
        maFreezeCells[i]->updateSelf();
    }
    for (int i = 0; i < mMaxPrepareCellNum; i++) {
        maPrepareCells[i]->updateSelf();
    }
}

void KBlocksItemGroup::refreshItemByPos(const QList<int> &dataList)
{
    int posX = 0;
    int posY = 0;
    int pieceCellCount = dataList.size() / 2;
    for (int i = 0; i < pieceCellCount; i++) {
        posX = dataList[i * 2];
        posY = dataList[i * 2 + 1];
        if ((posX >= 0) && (posX < mFieldWidth)
                && (posY >= 0) && (posY < mFieldHeight)) {
            maFreezeCells[posX + posY * mFieldWidth]->updateSelf();
        }
    }
}

void KBlocksItemGroup::fadeInNewPiece()
{
    int count = mNewPiecePos.size() / 2;

    int posX[4] = { -1, -1, -1, -1};
    int posY[4] = { -1, -1, -1, -1};

    for (int i = 0; i < count; i++) {
        posX[i] = mNewPiecePos[i * 2];
        posY[i] = mNewPiecePos[i * 2 + 1];
    }

    mFadeInItems.clear();
    for (int i = 0; i < 4; i++) {
        if ((posX[i] >= 0 && posX[i] < mFieldWidth)
                && (posY[i] >= 0 && posY[i] < mFieldHeight)) {
            maFreezeCells[posX[i] + posY[i] * mFieldWidth]->setOpacity(0);
            mFadeInItems.append(maFreezeCells[posX[i] + posY[i] * mFieldWidth]);
        }
    }

    for (int i = 0; i < mMaxFreezeCellNum; i++) {
        maFreezeCells[i]->updateSelf();
    }
    for (SvgItemInterface *tmpItem : qAsConst(mFadeOutItems)) {
        tmpItem->setOpacity(1);
        tmpItem->stopOpAnim();
    }
    for (SvgItemInterface *tmpItem : qAsConst(mDropItems)) {
        tmpItem->stopPosAnim();
    }

    for (int i = 0; i < PREPARE_AREA_WIDTH * PREPARE_AREA_WIDTH; i++) {
        maPrepareCells[i]->updateSelf();
        if (maPrepareCells[i]->isVisible()) {
            maPrepareCells[i]->setOpacity(0);
            mFadeInItems.append(maPrepareCells[i]);
        }
    }

    mpAnimator->createFadeAnim(mFadeInItems, FADE_ANIM_TIME_LINE, QTimeLine::Forward);
}

void KBlocksItemGroup::fadeOutOldLine()
{
    int count = mRemovedLine.size();

    mFadeOutItems.clear();
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < mFieldWidth; j++) {
            maFreezeCells[j + mRemovedLine[i] * mFieldWidth]->startOpAnim();
            mFadeOutItems.append(maFreezeCells[j + mRemovedLine[i] * mFieldWidth]);
        }
    }

    for (int i = 0; i < PREPARE_AREA_WIDTH * PREPARE_AREA_WIDTH; i++) {
        if (maPrepareCells[i]->isVisible()) {
            mFadeOutItems.append(maPrepareCells[i]);
        }
    }

    mpAnimator->createFadeAnim(mFadeOutItems, FADE_ANIM_TIME_LINE, QTimeLine::Backward);
}

void KBlocksItemGroup::dropFreezeLine()
{
    int count = mRemovedLine.size();

    int *fallLine = new int[mFieldHeight];
    int removeLine = 0;

    if (count == 0) {
        delete [] fallLine;
        return;
    }

    for (int i = mFieldHeight - 1; i >= 0; i--) {
        if (removeLine < count) {
            if (i == mRemovedLine[removeLine]) {
                fallLine[i] = 0;
                removeLine++;
            } else {
                fallLine[i] = removeLine;
            }
        } else {
            fallLine[i] = removeLine;
        }
    }

    mDropItems.clear();
    for (int i = 0; i < mFieldHeight; i++) {
        if (fallLine[i] > 0) {
            QPointF target;
            target.setX(0);
            target.setY(fallLine[i] * mItemSize);
            for (int j = 0; j < mFieldWidth; j++) {
                if (maFreezeCells[j + i * mFieldWidth]->isVisible()) {
                    maFreezeCells[j + i * mFieldWidth]->startPosAnim(target);
                    mDropItems.append(maFreezeCells[j + i * mFieldWidth]);
                }
            }
        }
    }

    mpAnimator->createDropAnim(mDropItems, DROP_ANIM_TIME_LINE, QTimeLine::Forward);
    delete[] fallLine;
}

void KBlocksItemGroup::updateGraphicInfo()
{
    mItemSize = mpGrafx->m_Block_Size;
    mPrepareLeft = mpGrafx->m_PreviewArea_CenterPoint_X - PREPARE_AREA_WIDTH * mpGrafx->m_Block_Size / 2;
    mPrepareTop = mpGrafx->m_PreviewArea_CenterPoint_Y - PREPARE_AREA_WIDTH * mpGrafx->m_Block_Size / 2;
    mFieldLeft = mpGrafx->m_PlayArea_OffsetPoint_X;
    mFieldTop = mpGrafx->m_PlayArea_OffsetPoint_Y;
    mFieldWidth = mpGrafx->m_PlayArea_NumberOfBlocks_X;
    mFieldHeight = mpGrafx->m_PlayArea_NumberOfBlocks_Y;
}
