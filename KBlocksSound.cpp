/***************************************************************************
 *   KBlocks, a falling blocks game by KDE                                *
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

#include <kgsound.h>

#include <QStandardPaths>

#include "kblocks_sound_debug.h"
#include "settings.h"

KBlocksSound::KBlocksSound(const QString &themeFile)
{
    loadTheme(themeFile);
}

KBlocksSound::~KBlocksSound()
{
    delete m_theme;
    delete m_blockFallSound;
    delete m_blockMoveSound;
    delete m_blockRemoveSound;
}

bool KBlocksSound::loadTheme(const QString &themeFile)
{
    m_theme = new KGameTheme();
    if (!m_theme->load(themeFile)) {
        qCWarning(KBSound) << "Error loading KBlocks .desktop theme"
                                   << themeFile << endl;
        m_theme->loadDefault();
    }

    QString m_themeMoveSound;
    if (m_theme->themeProperty("Sound_Block_Move") != "") {
        m_themeMoveSound = m_theme->prefix() + m_theme->themeProperty("Sound_Block_Move");
    } else {
        m_themeMoveSound = QStandardPaths::locate(
                    QStandardPaths::AppDataLocation, QStringLiteral("sounds/block-move.ogg"));
    }
    
    QString m_themeFallSound;
    if (m_theme->themeProperty("Sound_Block_Fall") != "") {
        m_themeFallSound = m_theme->prefix() + m_theme->themeProperty("Sound_Block_Fall");
    } else {
        m_themeFallSound = QStandardPaths::locate(
                    QStandardPaths::AppDataLocation, QStringLiteral("sounds/block-fall.ogg"));
    }
    
    QString m_themeRemoveSound;
    if (m_theme->themeProperty("Sound_Block_Remove") != "") {
        m_themeRemoveSound = m_theme->prefix() + m_theme->themeProperty("Sound_Block_Remove");
    } else {
        m_themeRemoveSound = QStandardPaths::locate(
                    QStandardPaths::AppDataLocation, QStringLiteral("sounds/block-remove.ogg"));
    }
    
    m_blockFallSound = new KgSound(m_themeFallSound);
    m_blockMoveSound = new KgSound(m_themeMoveSound);
    m_blockRemoveSound = new KgSound(m_themeRemoveSound);
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

