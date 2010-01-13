/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSNETSERVER_H
#define KBLOCKSNETSERVER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>

#include <map>
#include <list>
#include <string>

#include "KBlocksGameLogic.h"
#include "KBlocksScore.h"

using namespace std;

class KBlocksNetServer
{
    public:
        KBlocksNetServer(KBlocksGameLogic * p, const string& localIP);
        ~KBlocksNetServer();
        
    public:
        int  exec(int gameCount, bool waitForAll);
        
        void setTimeOut(int timeOut);
        void setSendLength(int initLen, int lvUpLen);
        
    private:
        int  recvRemoteData();
        int  processGame(int gameIndex, bool waitForAll);
        
        void addPlayerIP(int gameIndex, char * name);
        void delPlayerIP();
        
        void sendPlayerActionLength();
        void sendPlayerData(int gameIndex);
        void sendGameOver();
        void sendGuiData();
        
        int  parseRemoteData(char * data);
        int  parsePlayerReply(char * data);
        
        int  parseIPString(const string& input, string * ip, int * port);
        string formIPString(const sockaddr_in& inAddr);
        
        int  getDecIntFromString(const string& input);
        void formByteFromInt(int value, unsigned char * data);
        
        void printGameResult();
        
    private:
        KBlocksGameLogic* mpGameLogic;
        KBlocksScore** maGameScoreList;
        
        string mLocalIP;
        
        bool mSpeedMode;
        int mTimeOut;
        int mTopGameLevel;
        int mInitSendLength;
        int mLvUpSendLength;
        
        int mServerSocketFD;
        struct sockaddr_in mRemoteAddr;
        int mRemoteAddrLen;
        
        bool mRunningFlag;
        int mGameCount;
        bool mGameStarted;
        
        map<string, int> mPlayerMapping;
        map<int, string> mPlayerName;
        list<string> mPlayerIPList;
};

#endif
