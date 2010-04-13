/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2010 Mauricio Piacentini <mauricio@tabuleiro.com>       *
*                      Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksGraphics.h"
#include <KDebug>

#include <QPixmapCache>
#include <QPainter>

KBlocksGraphics::KBlocksGraphics ( const QString& themeFile )
{
    m_theme = new KGameTheme();
    if (!m_theme->load(themeFile)) {
        kDebug(11000) << "Error loading KBlocks .desktop theme" << themeFile << endl;
        m_theme->loadDefault();
    }
    m_renderer = new KSvgRenderer(m_theme->graphics());
    readThemeValues();
}

KBlocksGraphics::~KBlocksGraphics()
{
    delete m_theme;
    delete m_renderer;
}

bool KBlocksGraphics::loadTheme ( const QString& themeFile )
{
    if (!m_theme->load(themeFile)) {
        kDebug(11000) << "Error loading KBlocks .desktop theme" << themeFile << endl;
        return false;
    }
    if (!m_renderer->load(m_theme->graphics())) {
        kDebug(11000) << "Error loading SVG theme" << m_theme->graphics() << endl;
        return false;
    }
    //clear the cache or pixmaps from the old theme will be returned
    //QPixmapCache::clear();
    readThemeValues();
    return true;
}


void KBlocksGraphics::adjustForSize(const QSize& newsize)
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
    //kDebug(11000) << aspectratio;
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
    bounds = m_renderer->boundsOnElement ( "BLOCK_SIZE" );
    m_Block_Size = bounds.width();
    bounds = m_renderer->boundsOnElement ( "VIEW" );
    m_View_Size_Width = bounds.width();
    m_View_Size_Height = bounds.height();
    bounds = m_renderer->boundsOnElement ( "PLAY_AREA" );
    m_PlayArea_OffsetPoint_X = bounds.x();
    m_PlayArea_OffsetPoint_Y = bounds.y();
    m_PlayArea_NumberOfBlocks_X = bounds.width()/(double)m_Block_Size;
    m_PlayArea_NumberOfBlocks_Y = bounds.height()/(double)m_Block_Size;
    bounds = m_renderer->boundsOnElement ( "NEXTPIECE_AREA" );
    m_PreviewArea_CenterPoint_X = bounds.center().x();
    m_PreviewArea_CenterPoint_Y = bounds.center().y();
}

/*
QPixmap KBlocksGraphics::elementPixmap(int width, int height, const QString & elementid) {
    QPixmap pm;
    if (!QPixmapCache::find(pixmapCacheNameFromElementId(width, height, elementid), pm)) {
        pm = renderElement(width, height, elementid);
        QPixmapCache::insert(pixmapCacheNameFromElementId(width, height, elementid), pm);
    }
    return pm;
}

QPixmap KBlocksGraphics::renderElement(int width, int height, const QString & elementid) {
    QImage qiRend(QSize(width, height),QImage::Format_ARGB32_Premultiplied);
    qiRend.fill(0);

    if (m_renderer->isValid()) {
        QPainter p(&qiRend);
        m_renderer->render(&p, elementid, QRectF(0,0,width,height));
    }
    return QPixmap::fromImage(qiRend);
}

QString KBlocksGraphics::pixmapCacheNameFromElementId(int width, int height, const QString & elementid) {
    return elementid + QString("W%1H%2").arg(width).arg(height);
}
*/

