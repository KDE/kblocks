/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
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

