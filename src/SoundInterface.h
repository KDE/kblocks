/*******************************************************************************
 *   KBlocks, a falling blocks game by KDE                                     *
 *   SPDX-FileCopyrightText: 2009-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
 *                                                                             *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ******************************************************************************/
#ifndef SOUNDINTERFACE_H
#define SOUNDINTERFACE_H

enum class Sound {
    BlockFall,
    BlockMove,
    BlockRemove
};

class SoundInterface
{
public:
    SoundInterface() = default;
    virtual ~SoundInterface() = default;

public:
    virtual bool loadTheme(const QString &) = 0;
    virtual void setSoundsEnabled(bool) = 0;
    virtual void playSound(Sound) = 0;
};

#endif // SOUNDINTERFACE_H
