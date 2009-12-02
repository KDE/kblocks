/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
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

#include "KBlocksGraphics.h"
#include "KBlocksSound.h"
#include "KBlocksSvgItem.h"
#include "KBlocksControl.h"
#include "KBlocksAnimator.h"

#include "SingleGameInterface.h"

#define  FADE_ANIM_TIME_LINE        250//1000//
#define  DROP_ANIM_TIME_LINE        250//1000//
#define  PREPARE_PIECE_INTERVAL     100

#define  INITIAL_UPDATE_INTERVAL    500
#define  LEVEL_UP_INTERVAL_STEP     25

#define  PREPARE_AREA_WIDTH         5

class KBlocksItemGroup : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    
    public:
        KBlocksItemGroup(SingleGameInterface * pS, KBlocksGraphics * pG);
        ~KBlocksItemGroup();
        
    public:
        void setControlHandler(KBlocksControl * p);
        void setSoundHandler(KBlocksSound * p);
        
        void setGroupID(int groupID);
        void refreshPosition(KBlocksGraphics * pG);
        
        void startGame();
        void pauseGame();
        void resumeGame();
        void stopGame();
        
        void gameLevelUp();
        
        void updateGroupItems();
        
        void prepareNextPiece();
        
    signals:
        void stepEmited(int groupID);
        
    public slots:
        void rotateCW();
        void rotateCCW();
        void moveLeft();
        void moveRight();
        void moveDown();
        void pushDown();
        
    private slots:
        void stepGame();
        void releasePiece();
        
        void endAnimation(int animType);
        
    private:
        void fadeInNewPiece();
        void fadeOutOldLine();
        void dropFreezeLine();
        
    private:
        int mGroupID;
        
        KBlocksSvgItem*  mpBackground;
        
        int mMaxFreezeCellNum;
        KBlocksSvgItem** maFreezeCells;
        KBlocksSvgItem** maPrepareCells;
        
        KBlocksLayout*  mpGameLayout;
        
        KBlocksControl* mpGameControl;
        
        KBlocksSound* mpSnd;
        
        QTimer mStepTimer;
        QTimer mReleaseTimer;
        int mUpdateInterval;
        
        KBlocksAnimator* mpAnimator;
        QList<KBlocksSvgItem*> mFadeInItems;
        QList<KBlocksSvgItem*> mFadeOutItems;
        QList<KBlocksSvgItem*> mDropItems;
        
        bool mPreparingPiece;
        
        int mRemovedLineCount;
        int mRemovedLineList[4];
        
        int mFieldWidth;
        int mFieldHeight;
        
        int mItemSize;
        int mPrepareLeft;
        int mPrepareTop;
        int mFieldLeft;
        int mFieldTop;
};

#endif
