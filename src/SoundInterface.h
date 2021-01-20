/*******************************************************************************
 *   KBlocks, a falling blocks game by KDE                                     *
 *   Copyright (C) 2009-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
 *                                                                             *
 *   This program is free software; you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by      *
 *   the Free Software Foundation; either version 2 of the License, or         *
 *   (at your option) any later version.                                       *
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
