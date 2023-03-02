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
class KgTheme;

enum class BackgroundLocation {
    Stretch,
    TopLeft,
    Center
};

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
          m_PreviewArea_CenterPoint_Y{},
          m_BackgroundLocation{}
    {}
    virtual ~GraphicsInterface() = default;

public:
    virtual bool loadTheme(const KgTheme *theme) = 0;
    virtual void readThemeValues(const KgTheme *theme) = 0;
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

    BackgroundLocation m_BackgroundLocation;
};

#endif // GRAPHICSINTERFACE_H
