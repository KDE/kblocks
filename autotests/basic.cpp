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
