/*******************************************************************************
 *   KBlocks, a falling blocks game by KDE                                     *
 *   Copyright (C) 2009-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
 *                                                                             *
 *   This program is free software; you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by      *
 *   the Free Software Foundation; either version 2 of the License, or         *
 *   (at your option) any later version.                                       *
 ******************************************************************************/
#ifndef MOCKSOUND_H
#define MOCKSOUND_H

#include "SoundInterface.h"

class MockSound : public SoundInterface
{
public:
    MockSound() = default;
    virtual ~MockSound() = default;

public:
    bool loadTheme(const QString &) override { return false; }
    void setSoundsEnabled(bool) override {}
    void playSound(Sound) override {}
};

#endif // MOCKSOUND_H
