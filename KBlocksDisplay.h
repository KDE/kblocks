/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSDISPLAY_H
#define KBLOCKSDISPLAY_H

#include <string>

#include <KMainWindow>
#include <QTimer>

#include "KBlocksScene.h"
#include "KBlocksView.h"

#include "KBlocksGameLogic.h"
#include "KBlocksNetClient.h"

class KBlocksDisplay : public KMainWindow
{
    Q_OBJECT
    
    public:
        KBlocksDisplay(int gameCount, const string& serverIP, int localPort);
        ~KBlocksDisplay();
        
    public:
        void setGamesPerLine(int count);
        void setUpdateInterval(int interval);
        
    public:
        void startDisplay();
        void stopDisplay();
        
    private:
        int  formIntFromByte(unsigned char * data);
        
    private slots:
        void updateGameDisplay();
        
    private:
        int mGameCount;
        int mGamesPerWidth;
        
        int mUpdateInterval;
        QTimer mUpdateTimer;
        
        KBlocksScene* mpGameScene;
        KBlocksView*  mpGameView;
        
        KBlocksGameLogic* mpGameLogic;
        KBlocksNetClient* mpNetClient;
};

#endif
