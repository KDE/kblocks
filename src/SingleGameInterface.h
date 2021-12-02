/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#ifndef SINGLE_GAME_INTERFACE
#define SINGLE_GAME_INTERFACE

#include "FieldInterface.h"
#include "PieceInterface.h"


class SingleGameInterface
{
public:
    SingleGameInterface()
        : mpField(nullptr), mPieceCount(0), mpPieceList(nullptr) {}
    virtual ~SingleGameInterface() {}

public:
    virtual FieldInterface *getField() = 0;

    virtual int getPieceCount() = 0;
    virtual PieceInterface *getPiece(int) = 0;

    virtual bool isActive() = 0;
    virtual bool isGameRunning() = 0;

    virtual int forceUpdateGame() = 0;
    virtual int updateGame() = 0;
    virtual int continueGame() = 0;

    virtual bool setCurrentPiece(int, int, int) = 0;

    virtual bool pickGameResult(int *) = 0;
    virtual bool pickGameAction(int *, int *) = 0;

protected:
    FieldInterface *mpField = nullptr;

    int mPieceCount;
    PieceInterface **mpPieceList = nullptr;
};

#endif //SINGLE_GAME_INTERFACE
