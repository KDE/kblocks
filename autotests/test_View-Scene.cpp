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
#include "KBlocksView.h"
#include "Testing/MockGameLogic.h"
#include "Testing/MockScene.h"

class testViewScene : public QObject
{
    Q_OBJECT
private slots:
    void test_settingsShouldBeUpdatedInViewAndScene();
};

void testViewScene::test_settingsShouldBeUpdatedInViewAndScene()
{
    /**
     * When the settings are updated for the View, the new settings
     * should also be read in the scene to make sure the scene fits
     * the view exactly.
     */
    MockGameLogic gameLogic;
    MockScene scene(&gameLogic);
    KBlocksView view(&scene);
    view.settingsChanged();
    QVERIFY(scene.readSettingsCalled);
}

QTEST_MAIN(testViewScene)

#include "test_View-Scene.moc"
