/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2010-2020 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/
#ifndef KBLOCKSGRAPHICS_H
#define KBLOCKSGRAPHICS_H

#include <QHash>
#include <QSvgRenderer>
#include <QPixmap>

#define USE_UNSTABLE_LIBKDEGAMESPRIVATE_API
#include <libkdegamesprivate/kgametheme.h>

#include "GraphicsInterface.h"

class KBlocksGraphics : public GraphicsInterface
{
public:
    explicit KBlocksGraphics(const QString &themeFile);
    virtual ~KBlocksGraphics();

public:
    bool loadTheme(const QString &themeFile) override;
    void readThemeValues() override;
    void adjustForSize(const QSize &newsize) override;
    KGameTheme *theme() const override
    {
        return m_theme;
    }
    QSvgRenderer *renderer() const override
    {
        return m_renderer;
    }

private:
    KGameTheme *m_theme = nullptr;
    QSvgRenderer *m_renderer = nullptr;

    //QPixmap renderElement(int width, int height, const QString & elementid);
    //QString pixmapCacheNameFromElementId(int width, int height, const QString & elementid);
};

#endif
