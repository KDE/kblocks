/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KBLOCKSTHEME_H
#define KBLOCKSTHEME_H

#include <KGameTheme>

// TODO: extend KGameTheme to provide look-up of relativ path with custom entries
// For now subclass and use the readFromDesktopFile override to estimate the
// sounds path from the custom data and the path of the desktop file available there.
class KBlocksTheme : public KGameTheme
{
    Q_OBJECT

public:
    Q_INVOKABLE KBlocksTheme(const QByteArray &identifier, QObject *parent = nullptr);
    ~KBlocksTheme() override;

    bool readFromDesktopFile(const QString& path) override;

    QString blockMoveSoundPath() const;
    QString blockFallSoundPath() const;
    QString blockRemoveSoundPath() const;

private:
    QString m_blockMoveSoundPath;
    QString m_blockFallSoundPath;
    QString m_blockRemoveSoundPath;
};

inline QString KBlocksTheme::blockMoveSoundPath() const { return m_blockMoveSoundPath; }
inline QString KBlocksTheme::blockFallSoundPath() const { return m_blockFallSoundPath; }
inline QString KBlocksTheme::blockRemoveSoundPath() const { return m_blockRemoveSoundPath; }

#endif

