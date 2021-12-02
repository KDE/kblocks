/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   SPDX-FileCopyrightText: 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#ifndef KBLOCKSAIPLAYER_H
#define KBLOCKSAIPLAYER_H

#include <string>
using namespace std;

#include "KBlocksAITypeDefine.h"

#include "../GamePlayerInterface.h"

#include "../SingleGameInterface.h"
#include "../KBlocksField.h"
#include "../KBlocksPiece.h"

#include "EvaluationInterface.h"
#include "PlannerInterface.h"

class KBlocksAIPlayer : public GamePlayerInterface
{
public:
    explicit KBlocksAIPlayer(const string &name = "");
    ~KBlocksAIPlayer() override;

public:
    void startGame(SingleGameInterface *p) override;
    void stopGame() override;

    void pauseGame(bool flag) override;

    void think(GamePlayer_ActionList *actionList) override;

    string getName() override;

private:
    string mAIName;

    // Private Control Data
    bool mAIStarted;
    bool mAIPaused;

    // Phase I   - State Update
    KBlocksField *mpAIField;
    KBlocksPiece *mpCurPiece;
    KBlocksPiece *mpNextPiece;
    void update();

    // Phase II  - Planning
    PlannerInterface *mpPlanner;
    int mNextCount;
    void planning();

    // Phase III - Situation Analysis
    EvaluationInterface *mpEvaluatorFinal;
    EvaluationInterface *mpEvaluatorFirst;
    EvaluationInterface *mpEvaluatorPre;
    void situationAdaption();

    // Phase IV  - States Evaluation
    int mBestPosition;
    int mBestRotation;
    void generateGoalAction();
};

#endif
