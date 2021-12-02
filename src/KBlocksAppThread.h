/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#ifndef KBLOCKSAPPTHREAD_H
#define KBLOCKSAPPTHREAD_H

#include <QThread>
#include "KBlocksPlayNetwork.h"

class KBlocksAppThread : public QThread
{
public:
    explicit KBlocksAppThread(KBlocksPlayNetwork *p);
    ~KBlocksAppThread() override;

public:
    void run() override;

private:
    KBlocksPlayNetwork *mPlayNetwork;
};

#endif
