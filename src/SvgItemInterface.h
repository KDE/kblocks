/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/
#ifndef SVGITEMINTERFACE_H
#define SVGITEMINTERFACE_H

#include <QGraphicsSvgItem>
#include <QPointF>

class SvgItemInterface : public QGraphicsSvgItem
{
public:
    SvgItemInterface() = default;
    virtual ~SvgItemInterface() = default;

public:
    virtual bool updateSelf() = 0;
    virtual void clearCache() = 0;

    virtual void startOpAnim() = 0;
    virtual void stopOpAnim() = 0;

    virtual void startPosAnim(QPointF target) = 0;
    virtual void execPosAnim(qreal step) = 0;
    virtual void stopPosAnim() = 0;
};

#endif // SVGITEMINTERFACE_H
