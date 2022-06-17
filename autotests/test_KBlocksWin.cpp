/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2009-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#include <QTest>

#include "KBlocksGameLogic.h"
#include "KBlocksWin.h"
#include "Testing/MockGameLogic.h"
#include "Testing/MockGraphics.h"
#include "Testing/MockScene.h"
#include "Testing/MockSound.h"
#include "Testing/TestingKBlocksWin.h"

class testKBlocksWin : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void callStopGameBeforeSingleGamesAreDeleted();
};

void testKBlocksWin::callStopGameBeforeSingleGamesAreDeleted()
{
    /**
     * The KBlocksItemGroups which are part of KBlocksScene contain a
     * pointer to the single game(s). Thus, KBlocksScene should not
     * be used after the single games have been deleted. This is in
     * particular true for the stopGame() method which processes the
     * final game action items.
     */
    MockGameLogic logic;
    MockGraphics graphics;
    MockSound sound;
    TestingKBlocksWin win(&logic, &graphics, &sound, nullptr, nullptr, 0, 0);
    MockScene *scene = new MockScene(&logic);
    win.replaceScene(scene);
    win.callStopGame();
    QVERIFY(!scene->singleGameDeletedWhenStopGameCalled);
}

QTEST_MAIN(testKBlocksWin)

#include "test_KBlocksWin.moc"
