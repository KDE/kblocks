/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2010 University Freiburg                                *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
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
    KBlocksAIPlayer(string name = "");
    ~KBlocksAIPlayer();
    
public:
    void startGame(SingleGameInterface * p);
    void stopGame();
    
    void pauseGame(bool flag);
    
    void think(GamePlayer_ActionList * actionList);
    
    string getName();
    
private:
    string mAIName;
    
    // Private Control Data
    bool mAIStarted;
    bool mAIPaused;
    
    // Phase I   - State Update
    SingleGameInterface* mpGame;
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
