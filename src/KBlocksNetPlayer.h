/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSNETPLAYER_H
#define KBLOCKSNETPLAYER_H

#include "KBlocksNetClient.h"
#include "KBlocksSingleGame.h"
#include "GamePlayerInterface.h"

class KBlocksNetPlayer
{
public:
    KBlocksNetPlayer(GamePlayerInterface *player, const string &serverIP, int localPort);
    ~KBlocksNetPlayer();

public:
    void joinGame(int gameIndex);
    void quitGame();

    void startGame(KBlocksSingleGame *p);
    void stopGame();

    bool execute();

private:
    int  formIntFromByte(char *data);

private:
    GamePlayerInterface *mpPlayer = nullptr;
    KBlocksSingleGame *mpGame = nullptr;
    KBlocksNetClient *mpNetClient = nullptr;

    int mSendLength;

    GamePlayer_ActionList mActionList;
};

#endif

