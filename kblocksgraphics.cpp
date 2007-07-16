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
  }
  m_renderer = new QSvgRenderer(m_theme->graphics());
}

KBlocksGraphics::~KBlocksGraphics()
{
  delete m_theme;
  delete m_renderer;
}

void KBlocksGraphics::setData(int key, const QVariant & value)
{
  m_data.insert ( key,value);
}

QVariant  KBlocksGraphics::data ( int key )
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


