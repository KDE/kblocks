/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2009-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/
#include <memory>

#include <QTest>

#include "Testing/MockGraphics.h"
#include "Testing/MockSingleGame.h"
#include "Testing/MockSound.h"
#include "Testing/MockSvgItem.h"

#include "Testing/TestingKBlocksItemGroup.h"

class testKBlocksItemGroup : public QObject
{
    Q_OBJECT
private slots:
    void refreshPositionShouldSetCorrectPositions();
    void updateGameShouldProcessGameActionsOnGameOver();
    void updateGameShouldRefreshItemsOnGameOver();
    void stopGameShouldProcessRemainingGameActions();
};

void testKBlocksItemGroup::refreshPositionShouldSetCorrectPositions()
{
    /**
     * When refreshPosition is called, the positions of the SVG items
     * in the prepare area and the playing field are updated. The new
     * positions are determined from the (possibly updated) layout.
     */
    MockSingleGame singleGame;
    MockGraphics graphics;
    graphics.m_PreviewArea_CenterPoint_X = 1;
    graphics.m_PreviewArea_CenterPoint_Y = 1;
    graphics.m_Block_Size = 1;
    graphics.m_PlayArea_NumberOfBlocks_X = 2;
    graphics.m_PlayArea_NumberOfBlocks_Y = 2;
    graphics.m_PlayArea_OffsetPoint_X = 1;
    graphics.m_PlayArea_OffsetPoint_Y = 1;
    MockSound sound;
    TestingKBlocksItemGroup itemGroup(0, &singleGame, &graphics, &sound);

    graphics.m_PreviewArea_CenterPoint_X = 30;
    graphics.m_PreviewArea_CenterPoint_Y = 50;
    graphics.m_Block_Size = 12;
    graphics.m_PlayArea_OffsetPoint_X = 10;
    graphics.m_PlayArea_OffsetPoint_Y = 15;

    itemGroup.refreshPosition();

    std::vector<QPointF> expectedPreparePositions{
        { 0, 20},
        {12, 20},
        {24, 20},
        {36, 20},
        {48, 20},
        { 0, 32},
        {12, 32},
        {24, 32},
        {36, 32}
    };

    for (size_t i = 0; i < expectedPreparePositions.size(); ++i) {
        auto *prepareCell = itemGroup.getPrepareCell(i);
        QVERIFY(prepareCell != nullptr);
        QCOMPARE(prepareCell->pos(), expectedPreparePositions.at(i));
    }

    std::vector<QPointF> expectedFreezePositions{
        {10, 15},
        {22, 15},
        {10, 27},
        {22, 27}
    };

    for (size_t i = 0; i < expectedFreezePositions.size(); ++i) {
        auto *freezeCell = itemGroup.getFreezeCell(i);
        QVERIFY(freezeCell != nullptr);
        QCOMPARE(freezeCell->pos(), expectedFreezePositions.at(i));
    }
}


void testKBlocksItemGroup::updateGameShouldProcessGameActionsOnGameOver()
{
    /**
     * When updateGame is called, updateLayout should be called even
     * when the game is over. Otherwise, some game actions are not
     * processed and the play area does not show where the last item
     * has moved, causing the game to be over.
     *
     * See bug 407244
     */
    std::unique_ptr<SingleGameInterface> pSingleGame( new MockSingleGame() );
    MockSingleGame *mock = dynamic_cast<MockSingleGame*>(pSingleGame.get());
    mock->updateGameReturnValue = 1;

    std::unique_ptr<GraphicsInterface> pGraphics( new MockGraphics() );
    std::unique_ptr<SoundInterface> pSound( new MockSound() );
    TestingKBlocksItemGroup itemGroup(0, pSingleGame.get(), pGraphics.get(), pSound.get() );

    itemGroup.callUpdateGame();

    QCOMPARE( mock->numberOfPickGameActionCalls, 1 );
}

void testKBlocksItemGroup::updateGameShouldRefreshItemsOnGameOver()
{
    /**
     * When updateGame is called, the final piece might just have been
     * placed, causing the game to be over. If refreshItems() is not
     * called, the final piece will be invisible. The correct behavior
     * is to fully display the final piece, making it clear how and
     * where it conflicted with the pieces on the field.
     */
    std::unique_ptr<SingleGameInterface> pSingleGame( new MockSingleGame() );
    std::unique_ptr<GraphicsInterface> pGraphics( new MockGraphics() );
    std::unique_ptr<SoundInterface> pSound( new MockSound() );
    TestingKBlocksItemGroup itemGroup(0, pSingleGame.get(), pGraphics.get(), pSound.get() );

    MockSvgItem *svgItem = new MockSvgItem();
    itemGroup.replaceFreezeCells(svgItem);

    itemGroup.callUpdateGame();

    QVERIFY(svgItem->updateSelfCalled);
}


void testKBlocksItemGroup::stopGameShouldProcessRemainingGameActions()
{
    /**
     * When the game is stopped (on game over), updateLayout needs to be
     * called. Otherwise, the play area is updated only after highscore
     * has been shown.
     *
     * See bug 407244
     */
    std::unique_ptr<SingleGameInterface> pSingleGame( new MockSingleGame() );
    std::unique_ptr<GraphicsInterface> pGraphics( new MockGraphics() );
    std::unique_ptr<SoundInterface> pSound( new MockSound() );
    KBlocksItemGroup itemGroup(0, pSingleGame.get(), pGraphics.get(), pSound.get() );

    itemGroup.stopGame();

    MockSingleGame *mock = dynamic_cast<MockSingleGame*>(pSingleGame.get());
    QCOMPARE( mock->numberOfPickGameActionCalls, 1 );
}


QTEST_MAIN(testKBlocksItemGroup)

#include "test_KBlocksItemGroup.moc"
