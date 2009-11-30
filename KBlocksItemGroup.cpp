#include "KBlocksItemGroup.h"

#include <KDebug>

KBlocksItemGroup::KBlocksItemGroup(SingleGameInterface * pS, KBlocksGraphics * pG)
{
    mItemSize = pG->m_Block_Size;
    mPrepareLeft = pG->m_PreviewArea_CenterPoint_X - PREPARE_AREA_WIDTH * pG->m_Block_Size / 2;
    mPrepareTop = pG->m_PreviewArea_CenterPoint_Y - PREPARE_AREA_WIDTH * pG->m_Block_Size / 2;
    mFieldLeft = pG->m_PlayArea_OffsetPoint_X;
    mFieldTop = pG->m_PlayArea_OffsetPoint_Y;
    mFieldWidth = (pS->getField())->getWidth();
    mFieldHeight = (pS->getField())->getHeight();
    
    mpGameLayout = new KBlocksLayout();
    mpGameLayout->setGameField(pS->getField());
    mpGameLayout->setActivePiece(pS->getPiece(0));
    mpGameLayout->setNextPiece(pS->getPiece(1));
    mpGameLayout->initLayout();
    
    mpBackground = new KBlocksSvgItem();
    mpBackground->setLayoutHandler(mpGameLayout);
    mpBackground->setSharedRenderer(pG->renderer());
    mpBackground->setElementId("VIEW");
    addToGroup(mpBackground);
    
    maPrepareCells = new KBlocksSvgItem*[PREPARE_AREA_WIDTH * PREPARE_AREA_WIDTH]();
    for(int i = 0; i < PREPARE_AREA_WIDTH * PREPARE_AREA_WIDTH; i++)
    {
        maPrepareCells[i] = new KBlocksSvgItem();
        maPrepareCells[i]->setLayoutHandler(mpGameLayout);
        maPrepareCells[i]->setLayoutPos(i % PREPARE_AREA_WIDTH, i / PREPARE_AREA_WIDTH);
        maPrepareCells[i]->setLayoutType(KBlocksSvgItem_PrepareArea);
        maPrepareCells[i]->setSharedRenderer(pG->renderer());
        maPrepareCells[i]->setElementId("BLOCK_0");
        maPrepareCells[i]->setVisible(false);
        addToGroup(maPrepareCells[i]);
    }
    
    mMaxFreezeCellNum = (mFieldWidth * mFieldHeight);
    maFreezeCells = new KBlocksSvgItem*[mMaxFreezeCellNum]();
    for(int i = 0; i < mMaxFreezeCellNum; i++)
    {
        maFreezeCells[i] = new KBlocksSvgItem();
        maFreezeCells[i]->setLayoutHandler(mpGameLayout);
        maFreezeCells[i]->setLayoutPos(i % mFieldWidth, i / mFieldWidth);
        maFreezeCells[i]->setLayoutType(KBlocksSvgItem_FieldArea);
        maFreezeCells[i]->setSharedRenderer(pG->renderer());
        maFreezeCells[i]->setElementId("BLOCK_0");
        maFreezeCells[i]->setVisible(false);
        addToGroup(maFreezeCells[i]);
    }
    
    mpGameControl = 0;
    mpSnd = 0;
    
    mStepTimer.stop();
    mReleaseTimer.stop();
    
    mpAnimator = new KBlocksAnimator();
    connect(mpAnimator, SIGNAL(animFinished(int)), this, SLOT(endAnimation(int)));
    
    mPreparingPiece = true;
    
    mFadeInItems.clear();
    mFadeOutItems.clear();
    mDropItems.clear();
    
}

KBlocksItemGroup::~KBlocksItemGroup()
{
    delete mpAnimator;
    
    for(int i = 0; i < mMaxFreezeCellNum; i++)
    {
        removeFromGroup(maFreezeCells[i]);
        delete maFreezeCells[i];
    }
    delete [] maFreezeCells;
    
    for(int i = 0; i < PREPARE_AREA_WIDTH * PREPARE_AREA_WIDTH; i++)
    {
        removeFromGroup(maPrepareCells[i]);
        delete maPrepareCells[i];
    }
    delete [] maPrepareCells;
    
    removeFromGroup(mpBackground);
    delete mpBackground;
    
    mpGameLayout->freeLayout();
    delete mpGameLayout;
}

void KBlocksItemGroup::setControlHandler(KBlocksControl * p)
{
    mpGameControl = p;
}

void KBlocksItemGroup::setSoundHandler(KBlocksSound * p)
{
    mpSnd = p;
}

void KBlocksItemGroup::setGroupID(int groupID)
{
    mGroupID = groupID;
}

void KBlocksItemGroup::refreshPosition(KBlocksGraphics * pG)
{
    mItemSize = pG->m_Block_Size;
    mPrepareLeft = pG->m_PreviewArea_CenterPoint_X - PREPARE_AREA_WIDTH * pG->m_Block_Size / 2;
    mPrepareTop = pG->m_PreviewArea_CenterPoint_Y - PREPARE_AREA_WIDTH * pG->m_Block_Size / 2;
    mFieldLeft = pG->m_PlayArea_OffsetPoint_X;
    mFieldTop = pG->m_PlayArea_OffsetPoint_Y;
    
    mpBackground->setPos(0, 0);
    
    for(int i = 0; i < PREPARE_AREA_WIDTH * PREPARE_AREA_WIDTH; i++)
    {
        maPrepareCells[i]->setPos(mPrepareLeft + mItemSize * (i % PREPARE_AREA_WIDTH),
                                  mPrepareTop + mItemSize * (i / PREPARE_AREA_WIDTH));
    }
    
    for(int i = 0; i < mMaxFreezeCellNum; i++)
    {
        maFreezeCells[i]->setPos(mFieldLeft + mItemSize * (i % mFieldWidth),
                                 mFieldTop + mItemSize * (i / mFieldWidth));
    }
}

void KBlocksItemGroup::startGame()
{
    mUpdateInterval = INITIAL_UPDATE_INTERVAL;
    mStepTimer.setInterval(mUpdateInterval);
    connect(&mStepTimer, SIGNAL(timeout()), SLOT(stepGame()));
    mStepTimer.stop();
    
    mReleaseTimer.setSingleShot(true);
    connect(&mReleaseTimer, SIGNAL(timeout()), SLOT(releasePiece()));
    mReleaseTimer.stop();
    
    mReleaseTimer.start(INITIAL_UPDATE_INTERVAL);
    mPreparingPiece = true;
}

void KBlocksItemGroup::pauseGame()
{
    mReleaseTimer.stop();
    mStepTimer.stop();
    mPreparingPiece = true;
}

void KBlocksItemGroup::resumeGame()
{
    mReleaseTimer.stop();
    mStepTimer.start();
    mPreparingPiece = false;
}

void KBlocksItemGroup::stopGame()
{
    mStepTimer.stop();
    mReleaseTimer.stop();
    mPreparingPiece = true;
}

void KBlocksItemGroup::gameLevelUp()
{
    mUpdateInterval -= LEVEL_UP_INTERVAL_STEP;
    if (mUpdateInterval < LEVEL_UP_INTERVAL_STEP * 2)
    {
        mUpdateInterval = LEVEL_UP_INTERVAL_STEP;
    }
    mStepTimer.setInterval(mUpdateInterval);
}

void KBlocksItemGroup::updateGroupItems()
{
    mpGameLayout->updateLayout(-1, 0);
    
    for(int i = 0; i < mMaxFreezeCellNum; i++)
    {
        maFreezeCells[i]->updateSelf();
    }
    for(int i = 0; i < PREPARE_AREA_WIDTH * PREPARE_AREA_WIDTH; i++)
    {
        maPrepareCells[i]->updateSelf();
    }
}

void KBlocksItemGroup::prepareNextPiece()
{
    mStepTimer.stop();
    
    mRemovedLineCount = mpGameControl->getRemovedLines(mRemovedLineList);
    
    fadeOutOldLine();
    dropFreezeLine();
    
    mPreparingPiece = true;
}

void KBlocksItemGroup::rotateCW()
{
    if (mPreparingPiece)
    {
        return;
    }
    if (mpGameControl->rotateCW())
    {
        if (mpSnd)
        {
            mpSnd->playSound("Sound_Block_Move");
        }
        updateGroupItems();
    }
}

void KBlocksItemGroup::rotateCCW()
{
    if (mPreparingPiece)
    {
        return;
    }
    if (mpGameControl->rotateCCW())
    {
        if (mpSnd)
        {
            mpSnd->playSound("Sound_Block_Move");
        }
        updateGroupItems();
    }
}

void KBlocksItemGroup::moveLeft()
{
    if (mPreparingPiece)
    {
        return;
    }
    if (mpGameControl->moveLeft())
    {
        if (mpSnd)
        {
            mpSnd->playSound("Sound_Block_Move");
        }
        updateGroupItems();
    }
}

void KBlocksItemGroup::moveRight()
{
    if (mPreparingPiece)
    {
        return;
    }
    if (mpGameControl->moveRight())
    {
        if (mpSnd)
        {
            mpSnd->playSound("Sound_Block_Move");
        }
        updateGroupItems();
    }
}

void KBlocksItemGroup::moveDown()
{
    if (mPreparingPiece)
    {
        return;
    }
    if (mpGameControl->moveDown())
    {
        if (mpSnd)
        {
            mpSnd->playSound("Sound_Block_Move");
        }
        updateGroupItems();
    }
}

void KBlocksItemGroup::pushDown()
{
    if (mPreparingPiece)
    {
        return;
    }
    if (mpGameControl->pushDown())
    {
        if (mpSnd)
        {
            mpSnd->playSound("Sound_Block_Move");
        }
        updateGroupItems();
    }
}

void KBlocksItemGroup::stepGame()
{
    emit stepEmited(mGroupID);
}

void KBlocksItemGroup::releasePiece()
{
    mStepTimer.start();
    mPreparingPiece = false;
}

void KBlocksItemGroup::endAnimation(int animType)
{
    switch(animType)
    {
        case KBlocks_Animation_Fade_In:
            mpAnimator->deleteFadeAnim();
            mReleaseTimer.start(PREPARE_PIECE_INTERVAL);
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

void KBlocksItemGroup::fadeInNewPiece()
{
    mpGameLayout->updateLayout(mRemovedLineCount, mRemovedLineList);
    
    int posX[4] = {-1, -1, -1, -1};
    int posY[4] = {-1, -1, -1, -1};
    
    mpGameControl->getNewPiecePosX(posX);
    mpGameControl->getNewPiecePosY(posY);
    
    mFadeInItems.clear();
    for(int i = 0; i < 4; i++)
    {
        if ((posX[i] >= 0 && posX[i] < mFieldWidth)
         && (posY[i] >= 0 && posY[i] < mFieldHeight))
        {
            maFreezeCells[posX[i] + posY[i] * mFieldWidth]->setOpacity(0);
            mFadeInItems.append(maFreezeCells[posX[i] + posY[i] * mFieldWidth]);
        }
    }
    
    for(int i = 0; i < mMaxFreezeCellNum; i++)
    {
        maFreezeCells[i]->updateSelf();
    }
    foreach(KBlocksSvgItem * tmpItem, mFadeOutItems)
    {
        tmpItem->setOpacity(1);
    }
    foreach(KBlocksSvgItem * tmpItem, mDropItems)
    {
        tmpItem->stopPosAnim();
    }
    
    for(int i = 0; i < PREPARE_AREA_WIDTH * PREPARE_AREA_WIDTH; i++)
    {
        maPrepareCells[i]->updateSelf();
        if (maPrepareCells[i]->isVisible())
        {
            maPrepareCells[i]->setOpacity(0);
            mFadeInItems.append(maPrepareCells[i]);
        }
    }
    
    mpAnimator->createFadeAnim(mFadeInItems, FADE_ANIM_TIME_LINE, QTimeLine::Forward);
}

void KBlocksItemGroup::fadeOutOldLine()
{
    mFadeOutItems.clear();
    for(int i = 0; i < mRemovedLineCount; i++)
    {
        for(int j = 0; j < mFieldWidth; j++)
        {
            mFadeOutItems.append(maFreezeCells[j + mRemovedLineList[i] * mFieldWidth]);
        }
    }
    
    for(int i = 0; i < PREPARE_AREA_WIDTH * PREPARE_AREA_WIDTH; i++)
    {
        if (maPrepareCells[i]->isVisible())
        {
            mFadeOutItems.append(maPrepareCells[i]);
        }
    }
    
    mpAnimator->createFadeAnim(mFadeOutItems, FADE_ANIM_TIME_LINE, QTimeLine::Backward);
}

void KBlocksItemGroup::dropFreezeLine()
{
    int fallLine[mFieldHeight];
    int removeLine = 0;
    
    if (mRemovedLineCount == 0)
    {
        return;
    }
    
    for(int i = mFieldHeight - 1; i >= 0; i--)
    {
        if (removeLine < mRemovedLineCount)
        {
            if (i == mRemovedLineList[removeLine])
            {
                fallLine[i] = 0;
                removeLine++;
            }
            else
            {
                fallLine[i] = removeLine;
            }
        }
        else
        {
            fallLine[i] = removeLine;
        }
    }
    
    mDropItems.clear();
    for(int i = 0; i < mFieldHeight; i++)
    {
        if (fallLine[i] > 0)
        {
            QPointF target;
            target.setX(0);
            target.setY(fallLine[i] * mItemSize);
            for(int j = 0; j < mFieldWidth; j++)
            {
                if (maFreezeCells[j + i * mFieldWidth]->isVisible())
                {
                    maFreezeCells[j + i * mFieldWidth]->startPosAnim(target);
                    mDropItems.append(maFreezeCells[j + i * mFieldWidth]);
                }
            }
        }
    }
    
    mpAnimator->createDropAnim(mDropItems, DROP_ANIM_TIME_LINE, QTimeLine::Forward);
}

