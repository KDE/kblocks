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
 //Uses routines from Kapman sound manager (game.cpp)

#include "KBlocksSound.h"
#include <KDebug>
#include <QFile>
#include <QFileInfo>

#define USE_UNSTABLE_LIBKDEGAMESPRIVATE_API
#include <libkdegamesprivate/kgametheme.h>

#include "settings.h"

KBlocksSound::KBlocksSound ( const QString& themeFile )
{
    m_media1 = 0;
    m_media2 = 0;
    m_theme = new KGameTheme();
    if (!m_theme->load(themeFile)) {
        kDebug(11000) << "Error loading KBlocks .desktop theme" << themeFile << endl;
        m_theme->loadDefault();
        kDebug(11000) << "Load default sound theme." << endl;
    }
    readThemeValues();
    setSoundsEnabled(Settings::sounds());
}

KBlocksSound::~KBlocksSound()
{
    delete m_theme;
    delete m_media1;
    delete m_media2;
}

bool KBlocksSound::loadTheme ( const QString& themeFile )
{
    if (!m_theme->load(themeFile)) {
        kDebug(11000) << "Error loading KBlocks .desktop theme" << themeFile << endl;
        return false;
    }
    readThemeValues();
    return true;
}

void KBlocksSound::readThemeValues()
{
    //Extract sound directory info
    //This functionality should be exposed by KGameTheme
    QFile themefile(m_theme->path());
    sndDirectory = QFileInfo(themefile).absolutePath() + '/';
    themefile.close();
}

void KBlocksSound::setSoundsEnabled(bool p_enabled) {
    sndActive = p_enabled;
    if (p_enabled)
    {
        if (!m_media1)
        {
            m_media1 = Phonon::createPlayer(Phonon::GameCategory);
        }
        if (!m_media2)
        {
            m_media2 = Phonon::createPlayer(Phonon::GameCategory);
        }
    }
    else
    {
        delete m_media1;
        delete m_media2;
        m_media1 = 0;
        m_media2 = 0;
    }
}

void KBlocksSound::playSound(const QString& p_soundkey) {
    Phonon::MediaObject* m_usedMedia;
    QString p_sound = sndDirectory+m_theme->themeProperty(p_soundkey);
    //kDebug(11000) << "Playing sound : " << p_sound << endl;
    if (sndActive)
    {
        // Choose the media object with the smallest remaining time
        if (m_media1->remainingTime() <= m_media2->remainingTime())
        {
            m_usedMedia = m_media1;
        }
        else
        {
            m_usedMedia = m_media2;
        }
        m_usedMedia->setCurrentSource(p_sound);
        m_usedMedia->play();
    }
}


