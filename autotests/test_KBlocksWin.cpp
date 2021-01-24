/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2009-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
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
private slots:
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
    TestingKBlocksWin win(&logic, &graphics, &sound, nullptr, 0, 0);
    MockScene *scene = new MockScene(&logic);
    win.replaceScene(scene);
    win.callStopGame();
    QVERIFY(!scene->singleGameDeletedWhenStopGameCalled);
}

QTEST_MAIN(testKBlocksWin)

#include "test_KBlocksWin.moc"
