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
#ifndef KBLOCKSGRAPHICS_H
#define KBLOCKSGRAPHICS_H

#include <QHash>
#include <QSvgRenderer>
#include <QPixmap>

#define USE_UNSTABLE_LIBKDEGAMESPRIVATE_API
#include <libkdegamesprivate/kgametheme.h>

class KBlocksGraphics
{
    public:
        explicit KBlocksGraphics( const QString& themeFile );
        virtual ~KBlocksGraphics();

    public:
        bool loadTheme ( const QString& themeFile );
        void readThemeValues();
        void adjustForSize(const QSize& newsize);
        KGameTheme * theme() { return m_theme; }
        QSvgRenderer * renderer() { return m_renderer; }
        //QPixmap elementPixmap(int width, int height, const QString & elementid);

        int m_Block_Size;
        int m_View_Size_Width;
        int m_View_Size_Height;
        int m_PlayArea_OffsetPoint_X;
        int m_PlayArea_OffsetPoint_Y;
        int m_PlayArea_NumberOfBlocks_X;
        int m_PlayArea_NumberOfBlocks_Y;
        int m_PreviewArea_CenterPoint_X;
        int m_PreviewArea_CenterPoint_Y;

    private:
        KGameTheme * m_theme;
        QSvgRenderer *m_renderer;

        //QPixmap renderElement(int width, int height, const QString & elementid);
        //QString pixmapCacheNameFromElementId(int width, int height, const QString & elementid);
};

#endif
