/*******************************************************************************
 *   KBlocks, a falling blocks game by KDE                                     *
 *   Copyright (C) 2009-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
 *                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
 *                           Julian Helfferich <julian.helfferich@mailbox.org> *
 *                                                                             *
 *   This program is free software; you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by      *
 *   the Free Software Foundation; either version 2 of the License, or         *
 *   (at your option) any later version.                                       *
 ******************************************************************************/
//Uses routines from Kapman sound manager (game.cpp)

#include "KBlocksSound.h"

#include <KgSound>

#include <QStandardPaths>

#include "kblocks_sound_debug.h"
#include "settings.h"

KBlocksSound::KBlocksSound(const QString &themeFile)
    : SoundInterface()
{
    loadTheme(themeFile);
}

KBlocksSound::~KBlocksSound()
{
    delete m_blockFallSound;
    delete m_blockMoveSound;
    delete m_blockRemoveSound;
}

bool KBlocksSound::loadTheme(const QString &themeFile)
{
    KGameTheme theme;
    if (!theme.load(themeFile)) {
        qCWarning(KBSound) << "Error loading KBlocks .desktop theme"
                                   << themeFile;
        theme.loadDefault();
    }

    QString themeMoveSound;
    if (!theme.themeProperty("Sound_Block_Move").isEmpty()) {
        themeMoveSound = theme.prefix() + theme.themeProperty("Sound_Block_Move");
    } else {
        themeMoveSound = QStandardPaths::locate(
                    QStandardPaths::AppDataLocation, QStringLiteral("sounds/block-move.ogg"));
    }
    
    QString themeFallSound;
    if (!theme.themeProperty("Sound_Block_Fall").isEmpty()) {
        themeFallSound = theme.prefix() + theme.themeProperty("Sound_Block_Fall");
    } else {
        themeFallSound = QStandardPaths::locate(
                    QStandardPaths::AppDataLocation, QStringLiteral("sounds/block-fall.ogg"));
    }
    
    QString themeRemoveSound;
    if (!theme.themeProperty("Sound_Block_Remove").isEmpty()) {
        themeRemoveSound = theme.prefix() + theme.themeProperty("Sound_Block_Remove");
    } else {
        themeRemoveSound = QStandardPaths::locate(
                    QStandardPaths::AppDataLocation, QStringLiteral("sounds/block-remove.ogg"));
    }
    
    delete m_blockFallSound;
    delete m_blockMoveSound;
    delete m_blockRemoveSound;
    m_blockFallSound = new KgSound(themeFallSound);
    m_blockMoveSound = new KgSound(themeMoveSound);
    m_blockRemoveSound = new KgSound(themeRemoveSound);
    return true;
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

