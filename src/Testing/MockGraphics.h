/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/
#ifndef MOCKGRAPHICS_H
#define MOCKGRAPHICS_H

#include <memory>

#include <QSvgRenderer>

#include "GraphicsInterface.h"

class QSize;
class QString;
class KGameTheme;

class MockGraphics : public GraphicsInterface
{
public:
    MockGraphics() : mRenderer(new QSvgRenderer())
    {
        m_PlayArea_NumberOfBlocks_X = 1;
        m_PlayArea_NumberOfBlocks_Y = 1;
    }
    virtual ~MockGraphics() = default;

public:
    bool loadTheme(const QString &) override { return false; }
    void readThemeValues() override {}
    void adjustForSize(const QSize &) override {}
    KGameTheme *theme() const override { return nullptr; }
    QSvgRenderer *renderer() const override { return mRenderer.get(); }

private:
    std::unique_ptr<QSvgRenderer> mRenderer;
};

#endif // MOCKGRAPHICS_H
