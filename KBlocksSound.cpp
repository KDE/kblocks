/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2009 Mauricio Piacentini <mauricio@tabuleiro.com>       *
 *                      Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
//Uses routines from Kapman sound manager (game.cpp)

#include "KBlocksSound.h"
#include "KBlocksDefine.h"
#include "settings.h"

#include <kgsound.h>

#include <QStandardPaths>

KBlocksSound::KBlocksSound()
{
    m_blockFallSound = new KgSound(QStandardPaths::locate(
                    QStandardPaths::DataLocation, "sounds/block-fall.ogg"));
    m_blockMoveSound = new KgSound(QStandardPaths::locate(
                    QStandardPaths::DataLocation, "sounds/block-move.ogg"));
    m_blockRemoveSound = new KgSound(QStandardPaths::locate(
                    QStandardPaths::DataLocation, "sounds/block-remove.ogg"));
    setSoundsEnabled(Settings::sounds());
}

KBlocksSound::~KBlocksSound()
{
    delete m_blockFallSound;
    delete m_blockMoveSound;
    delete m_blockRemoveSound;
}

void KBlocksSound::setSoundsEnabled(bool p_enabled)
{
    sndActive = p_enabled;
}

void KBlocksSound::playSound(Sound soundType)
{
    if (sndActive) {
        switch (soundType) {
        case Sound::BlockFall:
            m_blockFallSound->start();
            break;
        case Sound::BlockMove:
            m_blockMoveSound->start();
            break;
        case Sound::BlockRemove:
            m_blockRemoveSound->start();
            break;
        default:
            qCWarning(KBSound) << "Unknown Sound requested for playback.";
            break;
        }
    }
}

