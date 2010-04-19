/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSKEYBOARDPLAYER_H
#define KBLOCKSKEYBOARDPLAYER_H

#include <string>
using namespace std;

#include <KAction>
#include <KActionCollection>
#include <KMainWindow>
#include <KXmlGuiWindow>
#include <klocalizedstring.h>

#include "GamePlayerInterface.h"

class KBlocksKeyboardPlayer : public QObject, public GamePlayerInterface
{
    Q_OBJECT
    
    public:
        KBlocksKeyboardPlayer(KXmlGuiWindow * parent, string name = "", bool netMode = false);
        ~KBlocksKeyboardPlayer();
        
    public:
        void startGame(SingleGameInterface * p);
        void stopGame();
        
        void pauseGame(bool flag);
        
        void think(GamePlayer_ActionList * actionList);
        
        string getName();
        
    private:
        void bindKeys();
        
    private slots:
        void moveLeft();
        void moveRight();
        void moveDown();
        void pushDown();
        void rotateCW();
        void rotateCCW();
        
    protected:
        SingleGameInterface* mpGame;
        bool mPauseFlag;
        
        KAction* rotatecw;
        KAction* rotateccw;
        KAction* moveleft;
        KAction* moveright;
        KAction* movedown;
        KAction* pushdown;
        
    private:
        bool mNetMode;
        string mPlayerName;
        GamePlayer_ActionList mActionList;
        
        KXmlGuiWindow* mpKeyWindow;
        KActionCollection* mpKeyShortcuts;
};

#endif

