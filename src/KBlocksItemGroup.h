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

#ifndef KBLOCKSITEMGROUP_H
#define KBLOCKSITEMGROUP_H

#include <QGraphicsItemGroup>
#include <QTimer>
#include <QList>

#include "KBlocksAnimator.h"

#include "SingleGameInterface.h"

#include "KBlocksDefine.h"

class GraphicsInterface;
class KBlocksLayout;
class KBlocksSvgItem;
class SoundInterface;
class SvgItemInterface;

class KBlocksItemGroup : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

public:
    KBlocksItemGroup(int groupID, SingleGameInterface *p, GraphicsInterface *pG, SoundInterface *pS, bool snapshotMode = false);
    virtual ~KBlocksItemGroup();

public:
    void setUpdateInterval(int interval);
    void setGameAnimEnabled(bool flag);
    void setWaitForAllUpdate(bool flag);
    void refreshPosition();

    void startGame();
    void stopGame();

    void pauseGame(bool flag);

signals:
    void readyForAction(int groupID);

protected slots:
    void updateGame();

private slots:
    void updateSnapshot();
    void endAnimation(int animType);

private:
    bool updateLayout();
    void refreshItems();

    void refreshItemByPos(const QList<int> &dataList);

    void fadeInNewPiece();
    void fadeOutOldLine();
    void dropFreezeLine();

    void updateGraphicInfo();

protected:
    int mMaxFreezeCellNum;
    SvgItemInterface **maFreezeCells;

    int mMaxPrepareCellNum;
    SvgItemInterface **maPrepareCells;

private:
    int mGroupID;

    KBlocksSvgItem  *mpBackground;

    SingleGameInterface *mpSingleGame;
    KBlocksLayout *mpGameLayout = nullptr;
    GraphicsInterface *mpGrafx = nullptr;
    SoundInterface *mpSnd = nullptr;

    QTimer mUpdateTimer;
    int mUpdateInterval;
    bool mGameAnimEnabled;
    bool mWaitForAllUpdate;

    KBlocksAnimator *mpAnimator;
    QList<SvgItemInterface *> mFadeInItems;
    QList<SvgItemInterface *> mFadeOutItems;
    QList<SvgItemInterface *> mDropItems;

    QList<int> mRemovedLine;
    QList<int> mPunishLine;
    QList<int> mNewPiecePos;

    int mFieldWidth;
    int mFieldHeight;

    int mItemSize;
    int mPrepareLeft;
    int mPrepareTop;
    int mFieldLeft;
    int mFieldTop;
};

#endif

