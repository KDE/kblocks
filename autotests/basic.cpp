/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                  *
*   Copyright (C) 2021 Julian Helfferich <julian.helfferich@mailbox.org>   *
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
***************************************************************************/
#include <QTest>

class testBasic : public QObject
{
    Q_OBJECT
private slots:
    void testAlwaysSuccessful();
};

void testBasic::testAlwaysSuccessful()
{
    QVERIFY( true );
}

QTEST_MAIN(testBasic)

#include "basic.moc"
