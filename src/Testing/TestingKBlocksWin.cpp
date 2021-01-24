/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/

#include "TestingKBlocksWin.h"

TestingKBlocksWin::TestingKBlocksWin(
    GameLogicInterface *p,
    GraphicsInterface *graphics,
    SoundInterface *sound,
    KBlocksPlayManager *pM,
    int capacity,
    int gamecount
) : KBlocksWin(p, graphics, sound, pM, capacity, gamecount)
{
}

void TestingKBlocksWin::callStopGame()
{
    KBlocksWin::stopGame();
}

void TestingKBlocksWin::replaceScene(SceneInterface *newScene)
{
    disconnect(
        mpKBPlayer, &KBlocksKeyboardPlayer::blockMoved,
        mpGameScene, &SceneInterface::playMoveSound
    );
    disconnect(
        mpKBPlayer, &KBlocksKeyboardPlayer::blockDropped,
        mpGameScene, &SceneInterface::playDropSound
    );
    delete mpGameScene;
    mpGameScene = newScene;
    connect(
        mpKBPlayer, &KBlocksKeyboardPlayer::blockMoved,
        mpGameScene, &SceneInterface::playMoveSound
    );
    connect(
        mpKBPlayer, &KBlocksKeyboardPlayer::blockDropped,
        mpGameScene, &SceneInterface::playDropSound
    );
    mpGameView->setScene(mpGameScene);
}
