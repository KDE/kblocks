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

class testKBlocksGameLogic : public QObject
{
    Q_OBJECT
private slots:
    void test_hasSingleGames();
};

void testKBlocksGameLogic::test_hasSingleGames()
{
    /**
     * KBlocksGameLogic::hasSingleGames() should return true if single
     * games have been created and false otherwise.
     */
    KBlocksGameLogic gameLogic(1);
    QVERIFY(!gameLogic.hasSingleGames());
    gameLogic.startGame(1);
    QVERIFY(gameLogic.hasSingleGames());
    gameLogic.deleteSingleGames();
    QVERIFY(!gameLogic.hasSingleGames());

    gameLogic.startGame(0);
    QVERIFY(!gameLogic.hasSingleGames());
}

QTEST_MAIN(testKBlocksGameLogic)

#include "test_KBlocksGameLogic.moc"
