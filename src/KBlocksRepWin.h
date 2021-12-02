/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
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

class GraphicsInterface;
class SoundInterface;

class KBlocksRepWin : public KMainWindow
{
    Q_OBJECT

public:
    explicit KBlocksRepWin(
        GraphicsInterface *graphics,
        SoundInterface *sound,
        const char *replayFile,
        bool binaryMode = true
    );
    ~KBlocksRepWin() override;

public:
    void setGamesPerLine(int count);

    void setUpdateInterval(int interval);
    void setReplayStepLength(int stepLen);

    void setSnapshotFolder(const QString &folder);
    void setSnapshotFilename(const QString &fileName);

    bool replayLoaded();

public:
    void startReplay();
    void stopReplay();

private:
    QString getTimeString();
    void snapshotView();

private Q_SLOTS:
    void replayOneStep();

private:
    int mGameCount;
    //int mGamesPerWidth;

    int mUpdateInterval;
    QTimer mUpdateTimer;

    QString mSnapshotFolder;
    QString mSnapshotFilename;
    QPixmap mSnapshoter;

    KBlocksScene *mpGameScene = nullptr;
    KBlocksView  *mpGameView = nullptr;

    KBlocksGameLogic *mpGameLogic = nullptr;
    KBlocksGameReplayer *mpGameReplayer = nullptr;
};

#endif
