/*******************************************************************************
 *   KBlocks, a falling blocks game by KDE                                     *
 *   SPDX-FileCopyrightText: 2009-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
 *                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
 *                           Julian Helfferich <julian.helfferich@mailbox.org> *
 *                                                                             *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ******************************************************************************/
//Uses routines from Kapman sound manager (game.cpp)

#include "KBlocksSound.h"

#include "KBlocksTheme.h"

#include <KGameSound>

#include <QStandardPaths>

#include "kblocks_sound_debug.h"
#include "settings.h"

KBlocksSound::KBlocksSound(const KBlocksTheme *theme)
    : SoundInterface()
{
    loadTheme(theme);
}

KBlocksSound::~KBlocksSound()
{
    delete m_blockFallSound;
    delete m_blockMoveSound;
    delete m_blockRemoveSound;
}

bool KBlocksSound::loadTheme(const KBlocksTheme *theme)
{
    QString themeMoveSound = theme->blockMoveSoundPath();
    if (themeMoveSound.isEmpty()) {
        themeMoveSound = QStandardPaths::locate(
                    QStandardPaths::AppDataLocation, QStringLiteral("sounds/block-move.ogg"));
    }

    QString themeFallSound = theme->blockFallSoundPath();
    if (themeFallSound.isEmpty()) {
        themeFallSound = QStandardPaths::locate(
                    QStandardPaths::AppDataLocation, QStringLiteral("sounds/block-fall.ogg"));
    }
    
    QString themeRemoveSound = theme->blockRemoveSoundPath();
    if (themeRemoveSound.isEmpty()) {
        themeRemoveSound = QStandardPaths::locate(
                    QStandardPaths::AppDataLocation, QStringLiteral("sounds/block-remove.ogg"));
    }
    
    delete m_blockFallSound;
    delete m_blockMoveSound;
    delete m_blockRemoveSound;
    m_blockFallSound = new KGameSound(themeFallSound);
    m_blockMoveSound = new KGameSound(themeMoveSound);
    m_blockRemoveSound = new KGameSound(themeRemoveSound);
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

