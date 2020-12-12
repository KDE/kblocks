/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Mauricio Piacentini <mauricio@tabuleiro.com>       *
*                      Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksGraphics.h"
#include "kblocks_graphics_debug.h"

#include <QPixmapCache>

KBlocksGraphics::KBlocksGraphics(const QString &themeFile)
{
    m_theme = new KGameTheme();
    if (!m_theme->load(themeFile)) {
        qCWarning(KBGraphics) << "Error loading KBlocks .desktop theme"
                                   << themeFile;
        m_theme->loadDefault();
    }
    m_renderer = new QSvgRenderer(m_theme->graphics());
    readThemeValues();
}

KBlocksGraphics::~KBlocksGraphics()
{
    delete m_theme;
    delete m_renderer;
}

bool KBlocksGraphics::loadTheme(const QString &themeFile)
{
    if (!m_theme->load(themeFile)) {
        qCWarning(KBGraphics) << "Error loading KBlocks .desktop theme"
                                   << themeFile;
        return false;
    }
    if (!m_renderer->load(m_theme->graphics())) {
        qCWarning(KBGraphics) << "Error loading SVG theme"
                                   << m_theme->graphics();
        return false;
    }
    //clear the cache or pixmaps from the old theme will be returned
    //QPixmapCache::clear();
    readThemeValues();
    return true;
}

void KBlocksGraphics::adjustForSize(const QSize &newsize)
{
    Q_UNUSED(newsize)
    //Reset our values
    readThemeValues();

    return;

    /*
    double aspectratio;
    double nw = newsize.width();
    double nh = newsize.height();

    double origw = m_View_Size_Width;
    double origh = m_View_Size_Height;

    if ((origw/origh)>(nw/nh)) {
        //space will be left on height, use width as limit
        aspectratio = nw/origw;
    } else {
        aspectratio = nh/origh;
    }
    //qCDebug(KBGraphics) << aspectratio;
    m_Block_Size = (int) (aspectratio*(qreal)m_Block_Size);
    m_View_Size_Width = (int) (aspectratio*(double)m_View_Size_Width);
    m_View_Size_Height = (int) (aspectratio*(double)m_View_Size_Height);
    m_PlayArea_OffsetPoint_X = (int) (aspectratio*(qreal)m_PlayArea_OffsetPoint_X);
    m_PlayArea_OffsetPoint_Y = (int) (aspectratio*(qreal)m_PlayArea_OffsetPoint_Y);
    m_PreviewArea_CenterPoint_X = (int) (aspectratio*(qreal)m_PreviewArea_CenterPoint_X);
    m_PreviewArea_CenterPoint_Y = (int) (aspectratio*(qreal)m_PreviewArea_CenterPoint_Y);
    */
}

void KBlocksGraphics::readThemeValues()
{
    //Extract values from SVG elements
    QRectF bounds;
    bounds = m_renderer->boundsOnElement(QStringLiteral("BLOCK_SIZE"));
    m_Block_Size = bounds.width();
    bounds = m_renderer->boundsOnElement(QStringLiteral("VIEW"));
    m_View_Size_Width = bounds.width();
    m_View_Size_Height = bounds.height();
    bounds = m_renderer->boundsOnElement(QStringLiteral("PLAY_AREA"));
    m_PlayArea_OffsetPoint_X = bounds.x();
    m_PlayArea_OffsetPoint_Y = bounds.y();
    m_PlayArea_NumberOfBlocks_X = bounds.width() / (double)m_Block_Size;
    m_PlayArea_NumberOfBlocks_Y = bounds.height() / (double)m_Block_Size;
    bounds = m_renderer->boundsOnElement(QStringLiteral("NEXTPIECE_AREA"));
    m_PreviewArea_CenterPoint_X = bounds.center().x();
    m_PreviewArea_CenterPoint_Y = bounds.center().y();
}

