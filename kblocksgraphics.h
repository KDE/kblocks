/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2007 Mauricio Piacentini <mauricio@tabuleiro.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
#ifndef KBLOCKSGRAPHICS_H
#define KBLOCKSGRAPHICS_H

#include "kgametheme.h"
#include <QHash>
#include <QSvgRenderer>
#include <QPixmap>

class KBlocksGraphics
{
  public:
    explicit KBlocksGraphics( const QString& themeFile );
    virtual ~KBlocksGraphics();
    
  public:
    void setData(int key, const QVariant & value);
    QVariant data ( int key );
    KGameTheme * theme() { return m_theme; }
    QSvgRenderer * renderer() { return m_renderer; }
    QPixmap elementPixmap(short width, short height, const QString & elementid);
    
  private:
    QHash<int, QVariant> m_data;
    KGameTheme * m_theme;
    QSvgRenderer *m_renderer;
    
    QPixmap renderElement(short width, short height, const QString & elementid);
    QString pixmapCacheNameFromElementId(short width, short height, const QString & elementid);
};

enum BlockGraphicsData { Block_Size = 0, View_Width, View_Height, FieldOffset_Width, FieldOffset_Height, Field_Width,
   Field_Height };

#endif
