/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
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
