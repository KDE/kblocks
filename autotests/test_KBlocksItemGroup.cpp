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

#include "Testing/TestingKBlocksItemGroup.h"

class testKBlocksItemGroup : public QObject
{
    Q_OBJECT
private slots:
    void updateGameShouldProcessGameActionsOnGameOver();
    void stopGameShouldProcessRemainingGameActions();
};

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
