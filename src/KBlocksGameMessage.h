/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSGAMEMESSAGE_H
#define KBLOCKSGAMEMESSAGE_H

#include "KBlocksDefine.h"

class KBlocksGameMessage
{
public:
    explicit KBlocksGameMessage(int poolSize);
    ~KBlocksGameMessage();

public:
    bool pickGameResult(int *result);
    bool putGameResult(int result);
    void clearGameResult();

    bool pickGameAction(int *type, int *action);
    bool putGameAction(int type, int action);
    void clearGameAction();

private:
    int mPoolSize;

    int mActionCount;
    int *maActionType;
    int *maActionList;

    int mResultCount;
    int *maResultList;
};

#endif

