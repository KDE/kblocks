/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Mauricio Piacentini <mauricio@tabuleiro.com>       *
*                      Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#include "KBlocksGraphics.h"
#include "kblocks_graphics_debug.h"

#include <KgTheme>

#include <QPixmapCache>

KBlocksGraphics::KBlocksGraphics(const KgTheme *theme)
{
    m_renderer = new QSvgRenderer(theme->graphicsPath());
    readThemeValues(theme);
}

KBlocksGraphics::~KBlocksGraphics()
{
    delete m_renderer;
}

bool KBlocksGraphics::loadTheme(const KgTheme *theme)
{
    if (!m_renderer->load(theme->graphicsPath())) {
        qCWarning(KBGraphics) << "Error loading SVG theme"
                                   << theme->graphicsPath();
        return false;
    }
    //clear the cache or pixmaps from the old theme will be returned
    //QPixmapCache::clear();
    readThemeValues(theme);

    return true;
}

void KBlocksGraphics::readThemeValues(const KgTheme *theme)
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

    const QString backgroundLocation = theme->customData(QStringLiteral("BackgroundLocation"), QStringLiteral("Stretch"));
    if(backgroundLocation == QStringLiteral("Stretch")) {
        m_BackgroundLocation = BackgroundLocation::Stretch;
    } else if(backgroundLocation == QStringLiteral("TopLeft")) {
        m_BackgroundLocation = BackgroundLocation::TopLeft;
    } else if(backgroundLocation == QStringLiteral("Center")) {
        m_BackgroundLocation = BackgroundLocation::Center;
    }
}

