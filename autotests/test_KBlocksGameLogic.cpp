/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2009-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#include <QTest>

#include "KBlocksGameLogic.h"

class testKBlocksGameLogic : public QObject
{
    Q_OBJECT
private Q_SLOTS:
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
