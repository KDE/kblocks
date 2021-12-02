/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#ifndef MOCKSINGLEGAME_H
#define MOCKSINGLEGAME_H

#include <memory>

#include "MockField.h"
#include "MockPiece.h"
#include "SingleGameInterface.h"

class FieldInterface;
class PieceInterface;

class MockSingleGame : public SingleGameInterface
{
public:
    MockSingleGame()
        : SingleGameInterface(),
          numberOfPickGameActionCalls(0),
          updateGameReturnValue(0),
          mockField(new MockField()),
          mockPiece(new MockPiece())
    {}
    ~MockSingleGame() override = default;

public:
    FieldInterface *getField() override { return mockField.get(); }

    int getPieceCount() override { return 0; }
    PieceInterface *getPiece(int) override { return mockPiece.get(); }

    bool isActive() override { return false; }
    bool isGameRunning() override { return false; }

    int forceUpdateGame() override { return 0; }
    int updateGame() override { return updateGameReturnValue; }
    int continueGame() override { return 0; }

    bool setCurrentPiece(int, int, int) override { return false; }

    bool pickGameResult(int *) override { return false; }

    bool pickGameAction(int *, int *) override
    {
        ++numberOfPickGameActionCalls;
        return false;
    }

public:
    unsigned int numberOfPickGameActionCalls;
    int updateGameReturnValue;

private:
    std::unique_ptr<FieldInterface> mockField;
    std::unique_ptr<PieceInterface> mockPiece;
};

#endif //SINGLE_GAME_INTERFACE
