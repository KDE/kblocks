/*******************************************************************************
 *   KBlocks, a falling blocks game by KDE                                     *
 *   SPDX-FileCopyrightText: 2009-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
 *                                                                             *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ******************************************************************************/
#ifndef MOCKSOUND_H
#define MOCKSOUND_H

#include "SoundInterface.h"

class MockSound : public SoundInterface
{
public:
    MockSound() = default;
    ~MockSound() override = default;

public:
    bool loadTheme(const KBlocksTheme *) override { return false; }
    void setSoundsEnabled(bool) override {}
    void playSound(Sound) override {}
};

#endif // MOCKSOUND_H
