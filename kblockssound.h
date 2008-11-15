/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2008 Mauricio Piacentini <piacentini@kde.org>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
#ifndef KBLOCKSSOUND_H
#define KBLOCKSSOUND_H

#include "kgametheme.h"
#include <QHash>
#include <Phonon/MediaObject>

class KBlocksSound
{
  public:
    explicit KBlocksSound( const QString& themeFile );
    virtual ~KBlocksSound();
    
  public:
    bool loadTheme ( const QString& themeFile );
    void readThemeValues();
    void setSoundsEnabled(bool p_enabled);
    void playSound(const QString& p_sound);
    KGameTheme * theme() { return m_theme; }
    
  private:
    KGameTheme * m_theme;
    QString sndDirectory;
    Phonon::MediaObject* m_media1;
    Phonon::MediaObject* m_media2;

};

  

#endif
