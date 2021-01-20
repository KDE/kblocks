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
#ifndef KBLOCKSSOUND_H
#define KBLOCKSSOUND_H

#define USE_UNSTABLE_LIBKDEGAMESPRIVATE_API
#include <libkdegamesprivate/kgametheme.h>

#include "SoundInterface.h"

class KgSound;

class KBlocksSound : public SoundInterface
{
public:
    explicit KBlocksSound(const QString &themeFile);
    virtual ~KBlocksSound();

public:
    bool loadTheme(const QString &themeFile) override;
    void setSoundsEnabled(bool p_enabled) override;
    void playSound(Sound soundType) override;

private:
    KgSound *m_blockFallSound = nullptr;
    KgSound *m_blockMoveSound = nullptr;
    KgSound *m_blockRemoveSound = nullptr;
    bool sndActive;

};

#endif
