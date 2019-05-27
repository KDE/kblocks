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

class KgSound;

enum class Sound {
    BlockFall,
    BlockMove,
    BlockRemove
};

class KBlocksSound
{
public:
    KBlocksSound();
    ~KBlocksSound();

public:
    void setSoundsEnabled(bool p_enabled);
    void playSound(Sound soundType);

private:
    KgSound *m_blockFallSound = nullptr;
    KgSound *m_blockMoveSound = nullptr;
    KgSound *m_blockRemoveSound = nullptr;
    bool sndActive;

};

#endif
