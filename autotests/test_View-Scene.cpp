/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2009-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#include <QTest>

#include "KBlocksScene.h"
#include "KBlocksView.h"
#include "Testing/MockGameLogic.h"
#include "Testing/MockScene.h"

class testViewScene : public QObject
{
    Q_OBJECT
private Q_SLOTS:
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
