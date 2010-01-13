/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksNetClient.h"

KBlocksNetClient::KBlocksNetClient(const string& remoteIP, int localPort)
{
    mRemoteIP = remoteIP;
    mTimeOut = 1000;
    
    string tmpIPAddress;
    int tmpPortNum;
    parseIPString(mRemoteIP, &tmpIPAddress, &tmpPortNum);
    
    mRemoteAddrLen = sizeof(mRemoteAddr);
    bzero(&mRemoteAddr, mRemoteAddrLen);
    mRemoteAddr.sin_family = AF_INET;
    mRemoteAddr.sin_addr.s_addr = inet_addr(tmpIPAddress.c_str());
    mRemoteAddr.sin_port = htons(tmpPortNum);
    
    mClientSocketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if (mClientSocketFD < 0)
    {
        printf("\tSocket error\n");
        return;
    }
    else
    {
        printf("\tSocket %d started.\n", mClientSocketFD);
    }
    
    struct sockaddr_in tmpLocalAddr;
    int tmpLocalAddrLen;
    tmpLocalAddrLen = sizeof(tmpLocalAddr);
    bzero(&tmpLocalAddr, tmpLocalAddrLen);
    tmpLocalAddr.sin_family = AF_INET;
    tmpLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    tmpLocalAddr.sin_port = htons(localPort);
    
    if (bind(mClientSocketFD, (struct sockaddr *)&tmpLocalAddr, tmpLocalAddrLen) < 0)
    {
        printf("\tBind error\n");
    }
    
    int r = fcntl(mClientSocketFD, F_GETFL, 0);
    fcntl(mClientSocketFD, F_SETFL, r & ~O_NONBLOCK);
}

KBlocksNetClient::~KBlocksNetClient()
{
    close(mClientSocketFD);
}

int KBlocksNetClient::sendData(int count, unsigned char * data)
{
    int ret = sendto(mClientSocketFD, data, count, 0, (struct sockaddr *)&mRemoteAddr, mRemoteAddrLen);
    if (ret < 0)
    {
        printf("Send error\n");
    }
    return ret;
}

int KBlocksNetClient::recvData(int count, unsigned char * data)
{
    int netResult;
    fd_set rd_set;
    
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = mTimeOut;
    
    do {
        FD_ZERO(&rd_set);
        FD_SET(mClientSocketFD, &rd_set);
        
        netResult = select((mClientSocketFD+1), &rd_set, NULL, NULL, &timeout);
        if (netResult < 0)
        {
            printf("Select error\n");
            return -3;
        }
        else if (netResult == 0)
        {
            return -1;
        }
        
        netResult = recvfrom(mClientSocketFD, data, count, 0, (struct sockaddr *)&mRemoteAddr, (socklen_t *)&mRemoteAddrLen);
        if (netResult < 0)
        {
            printf("Recv error\n");
            return -2;
        }
        else
        {
            return netResult;
        }
    } while (timeout.tv_usec > 0);
    
    return netResult;
}

void KBlocksNetClient::setTimeOut(int timeOut)
{
    mTimeOut = timeOut;
}

int KBlocksNetClient::parseIPString(const string& input, string * ip, int * port)
{
    *ip = input.substr(0, input.find(":"));
    return sscanf(input.substr(input.find(":") + 1).c_str(), "%d", port);
}
