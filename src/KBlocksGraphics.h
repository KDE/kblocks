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

#include <QSvgRenderer>
#include <QPixmap>

#include "GraphicsInterface.h"

class KBlocksGraphics : public GraphicsInterface
{
public:
    explicit KBlocksGraphics(const KgTheme *theme);
    ~KBlocksGraphics() override;

public:
    bool loadTheme(const KgTheme *theme) override;
    void readThemeValues(const KgTheme *theme) override;
    void adjustForSize(const QSize &newsize) override;
    QSvgRenderer *renderer() const override
    {
        return m_renderer;
    }

private:
    QSvgRenderer *m_renderer = nullptr;

    //QPixmap renderElement(int width, int height, const QString & elementid);
    //QString pixmapCacheNameFromElementId(int width, int height, const QString & elementid);
};

#endif
