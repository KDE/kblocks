/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#ifndef SVGITEMINTERFACE_H
#define SVGITEMINTERFACE_H

#include <QGraphicsSvgItem>
#include <QPointF>

class SvgItemInterface : public QGraphicsSvgItem
{
public:
    SvgItemInterface() = default;
    ~SvgItemInterface() override = default;

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
