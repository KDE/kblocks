/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
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
    ~KBlocksNetClient() override;

public:
    int sendData(int count, char *data);
    int recvData(int count, char *data);

Q_SIGNALS:
    void dataArrived(int size);

private:
    bool parseIPString(const QString &input, QHostAddress *ip, quint16 *port);

private Q_SLOTS:
    void receivedData();

private:
    QUdpSocket *mpClientSocket = nullptr;

    QHostAddress mLocalAddress;
    quint16 mLocalPort;

    QHostAddress mRemoteAddress;
    quint16 mRemotePort;
};

#endif

