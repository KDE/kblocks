/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2007 Mauricio Piacentini <mauricio@tabuleiro.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
#include "kblocksgraphics.h"
#include <KDebug>

#include <QVariant>
#include <QPixmapCache>
#include <QPainter>

KBlocksGraphics::KBlocksGraphics ( const QString& themeFile )
{
  m_theme = new KGameTheme();
  if (!m_theme->load(themeFile)) {
    kDebug(11000) << "Error loading KBlocks .desktop theme" << themeFile << endl;
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
  readThemeValues();
  return true;
}

void KBlocksGraphics::adjustForSize(const QSize& newsize)
{
  //Reset our values
  readThemeValues();

  qreal aspectratio;
  qreal nw = newsize.width();
  qreal nh = newsize.height();

  qreal origw = data(View_Size_Width);
  qreal origh = data(View_Size_Height);

  if ((origw/origh)>(nw/nh)) {
    //space will be left on height, use width as limit
    aspectratio = nw/origw;
  } else {
    aspectratio = nh/origh;
  }
  //kDebug(11000) << aspectratio;
  setData(Block_Size, aspectratio*data(Block_Size));
  setData(View_Size_Width, aspectratio*data(View_Size_Width));
  setData(View_Size_Height, aspectratio*data(View_Size_Height));
  setData(PlayArea_OffsetPoint_X, aspectratio*data(PlayArea_OffsetPoint_X));
  setData(PlayArea_OffsetPoint_Y, aspectratio*data(PlayArea_OffsetPoint_Y));
  setData(PreviewArea_CenterPoint_X, aspectratio*data(PreviewArea_CenterPoint_X));
  setData(PreviewArea_CenterPoint_Y, aspectratio*data(PreviewArea_CenterPoint_Y));
}

void KBlocksGraphics::readThemeValues()
{
  QStringList proplist;
  //Follow the BlockGraphicsData enum order
  proplist << "Block_Size" << "View_Size_Width" << "View_Size_Height" << "PlayArea_OffsetPoint_X" << "PlayArea_OffsetPoint_Y";
  proplist << "PlayArea_NumberOfBlocks_X" << "PlayArea_NumberOfBlocks_Y" << "PreviewArea_CenterPoint_X" << "PreviewArea_CenterPoint_Y";
  
  QString propString;
  bool ok;
  qreal propValue;
  int i=0;
  
  foreach (QString propKey, proplist) {
    propString = m_theme->themeProperty(propKey);
    propValue = propString.toFloat(&ok);
    setData(i, propValue);
    if (!ok) kDebug(11000) << "Bad layout data in KBlocks theme.";
    i++;
  }
  //kDebug(11000) << m_data;
}

void KBlocksGraphics::setData(int key, qreal value)
{
  m_data.insert ( key,value);
}

qreal KBlocksGraphics::data ( int key )
{
  return m_data.value(key);
}

QPixmap KBlocksGraphics::elementPixmap(short width, short height, const QString & elementid) {
  QPixmap pm;
  if (!QPixmapCache::find(pixmapCacheNameFromElementId(width, height, elementid), pm)) {
    pm = renderElement(width, height, elementid);
    QPixmapCache::insert(pixmapCacheNameFromElementId(width, height, elementid), pm);
  }
  return pm;
}

QPixmap KBlocksGraphics::renderElement(short width, short height, const QString & elementid) {
  QImage qiRend(QSize(width, height),QImage::Format_ARGB32_Premultiplied);
  qiRend.fill(0);

  if (m_renderer->isValid()) {
    QPainter p(&qiRend);
    m_renderer->render(&p, elementid);
  }
  return QPixmap::fromImage(qiRend);
}

QString KBlocksGraphics::pixmapCacheNameFromElementId(short width, short height, const QString & elementid) {
  return elementid + QString("W%1H%2").arg(width).arg(height);
}


