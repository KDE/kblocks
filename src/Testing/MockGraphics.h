/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#ifndef MOCKGRAPHICS_H
#define MOCKGRAPHICS_H

#include <memory>

#include <QSvgRenderer>

#include "GraphicsInterface.h"

class QSize;
class QString;

class MockGraphics : public GraphicsInterface
{
public:
    MockGraphics() : mRenderer(new QSvgRenderer())
    {
        m_PlayArea_NumberOfBlocks_X = 1;
        m_PlayArea_NumberOfBlocks_Y = 1;
    }
    ~MockGraphics() override = default;

public:
    bool loadTheme(const KgTheme *) override { return false; }
    void readThemeValues(const KgTheme *) override {}
    QSvgRenderer *renderer() const override { return mRenderer.get(); }

private:
    std::unique_ptr<QSvgRenderer> mRenderer;
};

#endif // MOCKGRAPHICS_H
