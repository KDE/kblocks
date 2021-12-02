/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2020 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
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
    ~KBlocksGraphics() override;

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
