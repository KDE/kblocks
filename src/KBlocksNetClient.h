/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSNETCLIENT_H
#define KBLOCKSNETCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QString>

class KBlocksNetClient : public QObject
{
    Q_OBJECT

public:
    KBlocksNetClient(const QString &remoteIP, quint16 localPort);
    ~KBlocksNetClient();

public:
    int sendData(int count, char *data);
    int recvData(int count, char *data);

signals:
    void dataArrived(int size);

private:
    bool parseIPString(const QString &input, QHostAddress *ip, quint16 *port);

private slots:
    void receivedData();

private:
    QUdpSocket *mpClientSocket = nullptr;

    QHostAddress mLocalAddress;
    quint16 mLocalPort;

    QHostAddress mRemoteAddress;
    quint16 mRemotePort;
};

#endif

