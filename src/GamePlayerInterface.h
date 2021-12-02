/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#ifndef GAMEPLAYERINTERFACE_H
#define GAMEPLAYERINTERFACE_H

#include <list>
#include <string>

enum KBlocks_Player_Action {
    PlayerAction_None = 0,
    PlayerAction_Move_Left,
    PlayerAction_Move_Right,
    PlayerAction_Move_Down,
    PlayerAction_Push_Down,
    PlayerAction_Rotate_CW,
    PlayerAction_Rotate_CCW,
    PlayerAction_Max_Count
};

typedef std::list<KBlocks_Player_Action> GamePlayer_ActionList;

#include "SingleGameInterface.h"

class GamePlayerInterface
{
public:
    GamePlayerInterface() : mpGame(nullptr) {};
    virtual ~GamePlayerInterface() {};

public:
    virtual void startGame(SingleGameInterface *) = 0;
    virtual void stopGame() = 0;

    virtual void pauseGame(bool) = 0;

    virtual void think(GamePlayer_ActionList *actionList) = 0;

    virtual std::string getName() = 0;

protected:
    SingleGameInterface *mpGame;
};

#endif //GAMEPLAYERINTERFACE_H
