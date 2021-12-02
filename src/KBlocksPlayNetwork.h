/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#ifndef KBLOCKSPLAYNETWORK_H
#define KBLOCKSPLAYNETWORK_H

#include "KBlocksGameLogic.h"
#include "GamePlayerInterface.h"
#include "KBlocksNetPlayer.h"

class KBlocksPlayNetwork
{
public:
    KBlocksPlayNetwork(int capacity, const string &serverIP, int localPort);
    ~KBlocksPlayNetwork();

public:
    bool addGamePlayer(GamePlayerInterface *p);
    void clearGamePlayer();

    void startGame();
    void stopGame();

    int  execute();

    void cancelExecute();

private:
    string mServerIP;
    int mLocalPort;

    bool mRunning;

    int mPlayerCount;
    int mMaxCapacity;
    KBlocksNetPlayer **maPlayerList;

    KBlocksGameLogic *mpGameLogic = nullptr;
};

#endif

