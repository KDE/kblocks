/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksGameMessage.h"

KBlocksGameMessage::KBlocksGameMessage(int poolSize)
{
    mPoolSize = poolSize;

    mActionCount = 0;
    maActionType = new int[mPoolSize];
    maActionList = new int[mPoolSize];

    mResultCount = 0;
    maResultList = new int[mPoolSize];
}

KBlocksGameMessage::~KBlocksGameMessage()
{
    delete [] maResultList;
    delete [] maActionList;
    delete [] maActionType;
}

bool KBlocksGameMessage::pickGameResult(int *result)
{
    if (mResultCount == 0) {
        return false;
    }

    *result = maResultList[0];

    for (int j = 0; j < mResultCount - 1; j++) {
        maResultList[j] = maResultList[j + 1];
    }

    mResultCount--;

    return true;
}

bool KBlocksGameMessage::putGameResult(int result)
{
    if (mResultCount == mPoolSize) {
        return false;
    }

    maResultList[mResultCount] = result;

    mResultCount++;

    return true;
}

void KBlocksGameMessage::clearGameResult()
{
    mResultCount = 0;
}

bool KBlocksGameMessage::pickGameAction(int *type, int *action)
{
    if (mActionCount == 0) {
        return false;
    }

    if (*type != GameAction_None) {
        for (int i = 0; i < mActionCount; i++) {
            if (*type == maActionType[i]) {
                *action = maActionList[i];

                for (int j = i; j < mActionCount - 1; j++) {
                    maActionType[j] = maActionType[j + 1];
                    maActionList[j] = maActionList[j + 1];
                }

                mActionCount--;

                return true;
            }
        }

        return false;
    } else {
        *type = maActionType[0];
        *action = maActionList[0];

        for (int j = 0; j < mActionCount - 1; j++) {
            maActionType[j] = maActionType[j + 1];
            maActionList[j] = maActionList[j + 1];
        }

        mActionCount--;

        return true;
    }
}

bool KBlocksGameMessage::putGameAction(int type, int action)
{
    if (mActionCount == mPoolSize) {
        return false;
    }

    maActionType[mActionCount] = type;
    maActionList[mActionCount] = action;

    mActionCount++;

    return true;
}

void KBlocksGameMessage::clearGameAction()
{
    mActionCount = 0;
}
