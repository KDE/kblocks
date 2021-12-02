/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/

#ifndef TESTINGKBLOCKSWIN_H
#define TESTINGKBLOCKSWIN_H

#include "KBlocksWin.h"
#include "SceneInterface.h"

class GameLogicInterface;
class GraphicsInterface;
class KBlocksPlayManager;
class SoundInterface;

class TestingKBlocksWin : private KBlocksWin
{
public:
    TestingKBlocksWin(
        GameLogicInterface *p,
        GraphicsInterface *graphics,
        SoundInterface *sound,
        KBlocksPlayManager *pM,
        int capacity,
        int gamecount
    );
    ~TestingKBlocksWin() override = default;

    void callStopGame();
    void replaceScene(SceneInterface *newScene);
};

#endif // TESTINGKBLOCKSWIN_H
