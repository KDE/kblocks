/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#ifndef KBLOCKSSCENE_H
#define KBLOCKSSCENE_H

#include <QGraphicsSceneMouseEvent>
#include <QString>

#include <KGamePopupItem>

#include "KBlocksSound.h"
#include "KBlocksGraphics.h"
#include "KBlocksItemGroup.h"
#include "KBlocksScore.h"

#include "GameLogicInterface.h"

#include "KBlocksDefine.h"
#include "SceneInterface.h"

class SoundInterface;
class GraphicsInterface;

class KBlocksScene : public SceneInterface
{
    Q_OBJECT

public:
    explicit KBlocksScene(
        GameLogicInterface *p,
        GraphicsInterface *graphics,
        SoundInterface *sound,
        int capacity = 1
    );
    ~KBlocksScene() override;

    KBlocksItemGroup *getItemGroup(int index);
    KBlocksScore *getScoreHandler(int index);

    void createGameItemGroups(int groupCount, bool snapshotMode = false) override;
    void deleteGameItemGroups() override;

    void setGamesPerLine(int count) override;
    void setGameAnimEnabled(bool flag) override;
    void setWaitForAllUpdate(bool flag) override;
    void setUpdateInterval(int interval) override;
    void setSoundsEnabled(bool enabled) override;

    void readSettings() override;
    void loadTheme(const KBlocksTheme *theme) override;

    void startGame() override;
    void stopGame() override;

    void pauseGame(bool flag, bool fromUI = false) override;

    void addScore(int gameIndex, int lineCount) override;

private:
    void updateDimensions();

private Q_SLOTS:
    void greetPlayer();
    void gameOverPlayer();
    void gameOverMultiWin();
    void gameOverMultiLose();

    void showMessage(const QString &message, int ms);

    void updateGame();
    void readyForAction(int groupID);

public Q_SLOTS:
    void playMoveSound() override;
    void playDropSound() override;

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    GameLogicInterface *mpGameLogic = nullptr;
    bool mGameStarted;

    GraphicsInterface *mpGrafx = nullptr;
    SoundInterface *mpSnd = nullptr;

    int mSceneGamesPerLine;
    bool mGameAnimEnabled;
    bool mWaitForAllUpdate;
    bool *maGameReadySignal = nullptr;

    bool mSnapshotMode;

    int mTopGameLevel;

    int mMaxCapacity;
    int mGroupCount;
    KBlocksItemGroup **maGroupList = nullptr;
    KBlocksScore **maGameScoreList = nullptr;

    QSizeF mBackgroundSize;

    KGamePopupItem *mMessageBox = nullptr;

    int mUpdateInterval;
    QTimer mUpdateTimer;
};

#endif
