/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#include "KBlocksAppThread.h"

KBlocksAppThread::KBlocksAppThread(KBlocksPlayNetwork *p)
{
    mPlayNetwork = p;
}

KBlocksAppThread::~KBlocksAppThread()
{
}

void KBlocksAppThread::run()
{
    mPlayNetwork->execute();
}
