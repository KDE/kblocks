/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                  *
*   SPDX-FileCopyrightText: 2021 Julian Helfferich <julian.helfferich@mailbox.org>   *
*                                                                          *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#include <QTest>

class testBasic : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testAlwaysSuccessful();
};

void testBasic::testAlwaysSuccessful()
{
    QVERIFY( true );
}

QTEST_MAIN(testBasic)

#include "basic.moc"
