/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSREPWIN_H
#define KBLOCKSREPWIN_H

#include <KMainWindow>
#include <QTimer>
#include <QString>
#include <QPixmap>

#include "KBlocksScene.h"
#include "KBlocksView.h"
#include "KBlocksGameLogic.h"
#include "KBlocksGameReplayer.h"

using std::string;

class KBlocksRepWin : public KMainWindow
{
    Q_OBJECT
    
    public:
        KBlocksRepWin(const char * replayFile, bool binaryMode = true);
        ~KBlocksRepWin();
        
    public:
        void setGamesPerLine(int count);
        
        void setUpdateInterval(int interval);
        void setReplayStepLength(int stepLen);
        
        void setSnapshotFolder(const QString& folder);
        void setSnapshotFilename(const QString& fileName);
        
        bool replayLoaded();
        
    public:
        void startReplay();
        void stopReplay();
        
    private:
        QString getTimeString();
        void snapshotView();
        
    private slots:
        void replayOneStep();
        
    private:
        int mGameCount;
        int mGamesPerWidth;
        
        int mUpdateInterval;
        QTimer mUpdateTimer;
        
        QString mSnapshotFolder;
        QString mSnapshotFilename;
        QPixmap mSnapshoter;
        
        KBlocksScene* mpGameScene;
        KBlocksView*  mpGameView;
        
        KBlocksGameLogic* mpGameLogic;
        KBlocksGameReplayer* mpGameReplayer;
};

#endif
