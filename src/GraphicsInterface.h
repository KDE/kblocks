/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#ifndef GRAPHICSINTERFACE_H
#define GRAPHICSINTERFACE_H

class QSize;
class QString;
class QSvgRenderer;
class KGameTheme;

class GraphicsInterface
{
public:
    GraphicsInterface()
        : m_Block_Size{},
          m_View_Size_Width{},
          m_View_Size_Height{},
          m_PlayArea_OffsetPoint_X{},
          m_PlayArea_OffsetPoint_Y{},
          m_PlayArea_NumberOfBlocks_X{},
          m_PlayArea_NumberOfBlocks_Y{},
          m_PreviewArea_CenterPoint_X{},
          m_PreviewArea_CenterPoint_Y{}
    {}
    virtual ~GraphicsInterface() = default;

public:
    virtual bool loadTheme(const QString &) = 0;
    virtual void readThemeValues() = 0;
    virtual void adjustForSize(const QSize &) = 0;
    virtual KGameTheme *theme() const = 0;
    virtual QSvgRenderer *renderer() const = 0;

    int m_Block_Size;
    int m_View_Size_Width;
    int m_View_Size_Height;
    int m_PlayArea_OffsetPoint_X;
    int m_PlayArea_OffsetPoint_Y;
    int m_PlayArea_NumberOfBlocks_X;
    int m_PlayArea_NumberOfBlocks_Y;
    int m_PreviewArea_CenterPoint_X;
    int m_PreviewArea_CenterPoint_Y;
};

#endif // GRAPHICSINTERFACE_H
