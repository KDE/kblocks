/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#ifndef MOCKSVGITEM_H
#define MOCKSVGITEM_H

#include "SvgItemInterface.h"

class MockSvgItem : public SvgItemInterface
{
public:
    MockSvgItem() = default;

    ~MockSvgItem() override = default;

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
