/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
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

class KGameTheme;
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
    bool sndActive;

};

  

#endif
