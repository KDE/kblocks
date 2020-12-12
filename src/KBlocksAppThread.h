/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSAPPTHREAD_H
#define KBLOCKSAPPTHREAD_H

#include <QThread>
#include "KBlocksPlayNetwork.h"

class KBlocksAppThread : public QThread
{
public:
    explicit KBlocksAppThread(KBlocksPlayNetwork *p);
    ~KBlocksAppThread();

public:
    void run() override;

private:
    KBlocksPlayNetwork *mPlayNetwork;
};

#endif
