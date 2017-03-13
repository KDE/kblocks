/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
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
    *port = input.midRef(input.indexOf(QStringLiteral(":")) + 1).toUInt(&result);
    return result;
}

void KBlocksNetClient::receivedData()
{
    emit(dataArrived(mpClientSocket->pendingDatagramSize()));
}
