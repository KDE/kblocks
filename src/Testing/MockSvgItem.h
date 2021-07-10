/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/
#ifndef MOCKSVGITEM_H
#define MOCKSVGITEM_H

#include "SvgItemInterface.h"

class MockSvgItem : public SvgItemInterface
{
public:
    MockSvgItem() = default;

    virtual ~MockSvgItem() = default;

public:
    bool updateSelf() override { updateSelfCalled = true; return true; }
    void clearCache() override { clearCacheCalled = true; }
    void startOpAnim() override {}
    void stopOpAnim() override {}
    void startPosAnim(QPointF) override {}
    void execPosAnim(qreal) override {}
    void stopPosAnim() override {}

    bool updateSelfCalled = false;
    bool clearCacheCalled = false;
};

#endif //MOCKSVGITEM_H
