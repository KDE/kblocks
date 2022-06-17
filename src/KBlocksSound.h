/*******************************************************************************
 *   KBlocks, a falling blocks game by KDE                                     *
 *   SPDX-FileCopyrightText: 2009-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
 *                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
 *                           Julian Helfferich <julian.helfferich@mailbox.org> *
 *                                                                             *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ******************************************************************************/
#ifndef KBLOCKSSOUND_H
#define KBLOCKSSOUND_H

#include "SoundInterface.h"

class KgSound;

class KBlocksSound : public SoundInterface
{
public:
    explicit KBlocksSound(const KgTheme *theme);
    ~KBlocksSound() override;

public:
    bool loadTheme(const KgTheme *theme) override;
    void setSoundsEnabled(bool p_enabled) override;
    void playSound(Sound soundType) override;

private:
    KgSound *m_blockFallSound = nullptr;
    KgSound *m_blockMoveSound = nullptr;
    KgSound *m_blockRemoveSound = nullptr;
    bool sndActive;

};

#endif
