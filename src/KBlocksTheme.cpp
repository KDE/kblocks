/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "KBlocksTheme.h"

#include <QFileInfo>

KBlocksTheme::KBlocksTheme(const QByteArray &identifier, QObject *parent)
    : KGameTheme(identifier, parent)
{
}

KBlocksTheme::~KBlocksTheme() = default;

bool KBlocksTheme::readFromDesktopFile(const QString &path)
{
    // Base-class call.
    if (!KGameTheme::readFromDesktopFile(path)) {
        return false;
    }

    // Estimate the paths to sound files.
    // Always assume relative path for sound entries
    const QFileInfo fi(path);
    const QString themePath = fi.absolutePath() + QLatin1Char('/');

    m_blockMoveSoundPath = customData(QStringLiteral("Sound_Block_Move"));
    if (!m_blockMoveSoundPath.isEmpty()) {
        m_blockMoveSoundPath.prepend(themePath);
    }

    m_blockFallSoundPath = customData(QStringLiteral("Sound_Block_Fall"));
    if (!m_blockFallSoundPath.isEmpty()) {
        m_blockFallSoundPath.prepend(themePath);
    }

    m_blockRemoveSoundPath = customData(QStringLiteral("Sound_Block_Remove"));
    if (!m_blockRemoveSoundPath.isEmpty()) {
        m_blockRemoveSoundPath.prepend(themePath);
    }

    return true;
}

#include "moc_KBlocksTheme.cpp"
