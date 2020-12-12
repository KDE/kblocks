/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
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

