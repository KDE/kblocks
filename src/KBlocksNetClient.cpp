/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#include "KBlocksNetClient.h"

KBlocksNetClient::KBlocksNetClient(const QString &remoteIP, quint16 localPort)
{
    mLocalAddress = QHostAddress::Any;
    mLocalPort = localPort;

    parseIPString(remoteIP, &mRemoteAddress, &mRemotePort);

    mpClientSocket = new QUdpSocket(this);
    mpClientSocket->bind(mLocalAddress, mLocalPort);
    connect(mpClientSocket, &QUdpSocket::readyRead, this, &KBlocksNetClient::receivedData);
}

KBlocksNetClient::~KBlocksNetClient()
{
    delete mpClientSocket;
}

int KBlocksNetClient::sendData(int count, char *data)
{
    int ret = mpClientSocket->writeDatagram(data, count, mRemoteAddress, mRemotePort);
    if (ret < 0) {
        printf("Send error\n");
    }
    return ret;
}

int KBlocksNetClient::recvData(int count, char *data)
{
    if (!mpClientSocket->hasPendingDatagrams()) {
        return -1;
    }
    return mpClientSocket->readDatagram(data, count);
}

bool KBlocksNetClient::parseIPString(const QString &input, QHostAddress *ip, quint16 *port)
{
    bool result = false;
    ip->setAddress(input.left(input.indexOf(QStringLiteral(":"))));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    *port = input.midRef(input.indexOf(QStringLiteral(":")) + 1).toUInt(&result);
#else
    *port = QStringView(input).mid(input.indexOf(QStringLiteral(":")) + 1).toUInt(&result);
#endif
    return result;
}

void KBlocksNetClient::receivedData()
{
    Q_EMIT(dataArrived(mpClientSocket->pendingDatagramSize()));
}
