/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/

#ifndef TESTINGKBLOCKSWIN_H
#define TESTINGKBLOCKSWIN_H

#include "KBlocksWin.h"
#include "SceneInterface.h"

class GameLogicInterface;
class KBlocksPlayManager;

class TestingKBlocksWin : private KBlocksWin
{
public:
    TestingKBlocksWin(GameLogicInterface *p, KBlocksPlayManager *pM, int capacity, int gamecount);
    virtual ~TestingKBlocksWin() = default;

    void callStopGame();
    void replaceScene(SceneInterface *newScene);
};

#endif // TESTINGKBLOCKSWIN_H
