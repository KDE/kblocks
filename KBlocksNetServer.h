/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSNETSERVER_H
#define KBLOCKSNETSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QMap>
#include <QList>
#include <QString>
#include <QByteArray>

#include "KBlocksGameLogic.h"
#include "KBlocksScore.h"

using namespace std;

class KBlocksNetServer : public QObject
{
    Q_OBJECT

public:
    KBlocksNetServer(KBlocksGameLogic *p, const QString &localIP);
    ~KBlocksNetServer();

public:
    int  executeGame(int gameCount, bool waitForAll);

    void setSendLength(int initLen, int lvUpLen);
    void setRecordFile(const char *fileName, bool binaryMode = true);

private:
    void recvRemoteData(QVector<QByteArray> *recvData, QVector<QString> *recvAddr);
    int  processGame(int gameIndex);

    void addPlayerIP(int gameIndex, const QByteArray &data, const QString &addr);
    void delPlayerIP(const QString &addr);

    void sendPlayerActionLength();
    void sendPlayerData(int gameIndex);
    void sendGameOver();
    void sendGuiData(const QString &addr);

    int  parseRemoteData(const QByteArray &data, const QString &addr);
    int  parsePlayerReply(const QByteArray &data, const QString &addr);

    bool parseIPString(const QString &input, QHostAddress *ip, quint16 *port);
    QString formIPString(const QHostAddress &inAddr, quint16 inPort);
    void formByteFromInt(int value, char *data);

    void printGameResult();

private:
    KBlocksGameLogic *mpGameLogic;
    KBlocksScore **maGameScoreList;

    bool mWaitForAll;
    //bool mSpeedMode;
    //int mTimeOut;
    int mTopGameLevel;
    int mInitSendLength;
    int mLvUpSendLength;

    QHostAddress mLocalAddress;
    quint16 mLocalPort;
    QUdpSocket *mpServerSocket;

    QHostAddress mRemoteAddress;
    quint16 mRemotePort;

    bool mRunningFlag;
    int mGameCount;
    bool mGameStarted;

    QMap<QString, int> mPlayerMapping;
    QMap<int, QString> mPlayerName;
    QList<QString> mPlayerIPList;

    string mRecordFileName;
    bool mRecordFileType;
};

#endif

