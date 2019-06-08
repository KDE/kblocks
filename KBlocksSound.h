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
#ifndef KBLOCKSSOUND_H
#define KBLOCKSSOUND_H

#define USE_UNSTABLE_LIBKDEGAMESPRIVATE_API
#include <libkdegamesprivate/kgametheme.h>

class KgSound;

enum class Sound {
    BlockFall,
    BlockMove,
    BlockRemove
};

class KBlocksSound
{
public:
    KBlocksSound(const QString &themeFile);
    ~KBlocksSound();

public:
    bool loadTheme(const QString &themeFile);
    void setSoundsEnabled(bool p_enabled);
    void playSound(Sound soundType);
    KGameTheme *theme()
    {
        return m_theme;
    }

private:
    KgSound *m_blockFallSound = nullptr;
    KgSound *m_blockMoveSound = nullptr;
    KgSound *m_blockRemoveSound = nullptr;
    bool sndActive;
    KGameTheme *m_theme;

};

#endif
