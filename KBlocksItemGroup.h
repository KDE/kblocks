/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2010 Mauricio Piacentini <mauricio@tabuleiro.com>       *
*                      Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef KBLOCKSITEMGROUP_H
#define KBLOCKSITEMGROUP_H

#include <QGraphicsItemGroup>
#include <QTimer>
#include <QList>

#include "KBlocksGraphics.h"
#include "KBlocksSound.h"
#include "KBlocksSvgItem.h"
#include "KBlocksAnimator.h"

#include "SingleGameInterface.h"

#include "KBlocksDefine.h"

class KBlocksItemGroup : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    
    public:
        KBlocksItemGroup(int groupID, SingleGameInterface * p, KBlocksGraphics * pG, KBlocksSound * pS, bool snapshotMode = false);
        ~KBlocksItemGroup();
        
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
        
    private slots:
        void updateGame();
        void updateSnapshot();
        void endAnimation(int animType);
        
    private:
        bool updateLayout();
        void refreshItems();
        
        void refreshItemByPos(const QList<int> & dataList);
        
        void fadeInNewPiece();
        void fadeOutOldLine();
        void dropFreezeLine();
        
        void updateGraphicInfo();
        
    private:
        int mGroupID;
        
        KBlocksSvgItem*  mpBackground;
        
        int mMaxFreezeCellNum;
        KBlocksSvgItem** maFreezeCells;
        int mMaxPrepareCellNum;
        KBlocksSvgItem** maPrepareCells;
        
        SingleGameInterface* mpSingleGame;
        KBlocksLayout* mpGameLayout;
        KBlocksGraphics* mpGrafx;
        KBlocksSound* mpSnd;
        
        QTimer mUpdateTimer;
        int mUpdateInterval;
        bool mGameAnimEnabled;
        bool mWaitForAllUpdate;
        
        KBlocksAnimator* mpAnimator;
        QList<KBlocksSvgItem*> mFadeInItems;
        QList<KBlocksSvgItem*> mFadeOutItems;
        QList<KBlocksSvgItem*> mDropItems;
        
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

