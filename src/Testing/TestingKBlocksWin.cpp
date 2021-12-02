/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
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
