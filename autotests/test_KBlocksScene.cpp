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

#include "KBlocksScene.h"
#include "Testing/MockGameLogic.h"
#include "Testing/MockGraphics.h"
#include "Testing/MockSound.h"

class testKBlocksScene : public QObject
{
    Q_OBJECT
private slots:
    void redundantCallsToDeleteGameItemGroupsShouldBeAllowed();
};

void testKBlocksScene::redundantCallsToDeleteGameItemGroupsShouldBeAllowed()
{
    /**
     * The method deleteGameItemGroups() should remove all existing
     * KBlocksItemGroup objects as well as the message box. It should
     * be valid to call deleteGameItemGroups() multiple times under all
     * circumstances.
     *
     * Currently, this test is only a should-not-crash test. It does not
     * verify that the game item groups are indeed deleted.
     */
    MockGameLogic gameLogic;
    MockGraphics graphics;
    MockSound sound;
    KBlocksScene scene(&gameLogic, &graphics, &sound);

    scene.deleteGameItemGroups();
    scene.deleteGameItemGroups();
    scene.createGameItemGroups(1);
    scene.deleteGameItemGroups();
    scene.deleteGameItemGroups();
}

QTEST_MAIN(testKBlocksScene)

#include "test_KBlocksScene.moc"
