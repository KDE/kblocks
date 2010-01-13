/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSNETCLIENT_H
#define KBLOCKSNETCLIENT_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string>

using namespace std;

class KBlocksNetClient
{
    public:
        KBlocksNetClient(const string& remoteIP, int localPort);
        ~KBlocksNetClient();
        
    public:
        int sendData(int count, unsigned char * data);
        int recvData(int count, unsigned char * data);
        
        void setTimeOut(int timeOut);
        
    private:
        int parseIPString(const string& input, string * ip, int * port);
        
    private:
        string mRemoteIP;
        
        int mTimeOut;
        
        int mClientSocketFD;
        struct sockaddr_in mRemoteAddr;
        int mRemoteAddrLen;
};

#endif