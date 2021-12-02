/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
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

