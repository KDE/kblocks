/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   Copyright (C) 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
***************************************************************************/
#include "KBlocksAIPlayer.h"

#include "KBlocksAIEvaluation.h"
#include "KBlocksAIFeature.h"
#include "KBlocksAIPlannerExtend.h"
#include "KBlocksAILog.h"

#include "kblocks_ai_debug.h"

#include <utility>
#include <vector>

typedef std::pair<KBlocksPiece, double> Result;
typedef std::vector<Result> Result_Sequence;

#define MAX_UTILITY     // choose the goal with the highest utility
//#  PLANNER  ######################################
#define TWO_PIECE_PLANNER
//#  SITUATION TREE  ################################
#define SITUATION_MULTI_NARROW
#define SITUATION_CRITIC_HEIGHT
#define SITUATION_TERIS
//#define SITUATION_SECTION_ANALYSIS
//#define SITUATION_DEEP_NARROW
//#define SITUATION_TALL_BLOCKS

//#  EVALUATION FEATURE  ############################
//#define DIFFERENCE_EVALUATION
//#define SECOND_EVALUATION

//#  EXTRA EVALUATION   #############################
//#define SPECIAL_EVALUATION
#ifdef SPECIAL_EVALUATION
#define NBS_EVALUATION
#endif

KBlocksAIPlayer::KBlocksAIPlayer(const string &name)
{
    mAIName = name;

    mAIStarted = false;
    mAIPaused = false;

    mpAIField = 0;
    mpCurPiece = 0;
    mpNextPiece = 0;

    mpPlanner = 0;
    mNextCount = 0;

    mpEvaluatorFinal = 0;
    mpEvaluatorFirst = 0;
    mpEvaluatorPre = 0;

    mBestPosition = 0;
    mBestRotation = 0;
}

KBlocksAIPlayer::~KBlocksAIPlayer()
{
    if (mAIStarted) {
        delete mpPlanner;
        delete mpNextPiece;
        delete mpCurPiece;
        delete mpAIField;
    }
}

void KBlocksAIPlayer::startGame(SingleGameInterface *p)
{
    if (mAIStarted) {
        return;
    }

    mAIStarted = true;
    mpGame = p;

    mpAIField = new KBlocksField(mpGame->getField());
    mpCurPiece = new KBlocksPiece(mpGame->getPiece(0));
    mpNextPiece = new KBlocksPiece(mpGame->getPiece(1));

    mpPlanner = new KBlocksAIPlannerExtend(mpAIField);
}

void KBlocksAIPlayer::stopGame()
{
    if (!mAIStarted) {
        return;
    }

    delete mpPlanner;
    mpPlanner = 0;

    delete mpNextPiece;
    mpNextPiece = 0;
    delete mpCurPiece;
    mpCurPiece = 0;
    delete mpAIField;
    mpAIField = 0;

    mpGame = 0;
    mAIStarted = false;
}

void KBlocksAIPlayer::pauseGame(bool flag)
{
    mAIPaused = flag;
}

void KBlocksAIPlayer::think(GamePlayer_ActionList *actionList)
{
    if ((!mAIStarted) || (mAIPaused)) {
        return;
    }

    // Phase I   - State Update
    update();
    // Phase II  - Planning
    planning();
    // Phase III - Situation Analysis
    situationAdaption();
    // Phase IV  - States Evaluation
    generateGoalAction();

    // set up action list
    if (mBestRotation > 0) {
        for (int i = 0; i < mBestRotation; i++) {
            actionList->push_back(PlayerAction_Rotate_CW);
        }
    } else {
        mBestRotation = -mBestRotation;
        for (int i = 0; i < mBestRotation; i++) {
            actionList->push_back(PlayerAction_Rotate_CCW);
        }
    }

    if (mBestPosition > 0) {
        for (int i = 0; i < mBestPosition; i++) {
            actionList->push_back(PlayerAction_Move_Right);
        }
    } else {
        mBestPosition = -mBestPosition;
        for (int i = 0; i < mBestPosition; i++) {
            actionList->push_back(PlayerAction_Move_Left);
        }
    }

    actionList->push_back(PlayerAction_Push_Down);
}

string KBlocksAIPlayer::getName()
{
    return mAIName;
}

/******************************************************************
* THINK -  DECIDE - CATCH
*****************************************************************/
/* Phase I   - State Update     ----------------------------------*/
void KBlocksAIPlayer::update()
{
    mpAIField->copy(mpGame->getField());
    mpCurPiece->copy(mpGame->getPiece(0));
    mpNextPiece->copy(mpGame->getPiece(1));
}

/* Phase II  - Planning         ----------------------------------*/
void KBlocksAIPlayer::planning()
{
    if (!mpPlanner) {
        qCWarning(KBlocksAI) << "No planner set for AI evaluation!";
        return;
    }
#ifdef TWO_PIECE_PLANNER
    AIPlanner_PieceValue_Sequence mPieceSequence = AIPlanner_PieceValue_Sequence(0);
    mPieceSequence.push_back(KBlocks_PieceType_Detail(mpCurPiece->toValue()));
    mPieceSequence.push_back(KBlocks_PieceType_Detail(mpNextPiece->toValue()));
    KBlocksAIPlannerExtend *extendedPlanner = dynamic_cast<KBlocksAIPlannerExtend *>(mpPlanner);
    if (extendedPlanner != nullptr) {
        mNextCount = extendedPlanner->process(mPieceSequence);
    }
#else
    KBlocks_PieceType_Detail type = KBlocks_PieceType_Detail(mpCurPiece->toValue());
    mNextCount = mpPlanner->process(type);
#endif
}

/* Phase III - Situation Analysis               -------------------*/
void KBlocksAIPlayer::situationAdaption()
{
#ifndef SPECIAL_EVALUATION
    if (false) {
    }
#ifdef SITUATION_MULTI_NARROW
    else if (getDecisionFeature(DF_PEEKS_COUNT, mpAIField)) {
        mpEvaluatorFinal = WellsFillerEvaluation::instance();
    }
#endif
#ifdef SITUATION_CRITIC_HEIGHT
    else if (getDecisionFeature(DF_HEIGHT_MAX, mpAIField)) {
        mpEvaluatorFinal = HeightKillerEvaluation::instance();
    }
#endif
#ifdef SITUATION_SECTION_ANALYSIS
    else if (getDecisionFeature(DF_LAYER_SCAN, mpAIField)) {
        mpEvaluatorFinal = HalfBaseEvaluation::instance();
    }
#endif
#ifdef SITUATION_DEEP_NARROW
    else if (getDecisionFeature(DF_DEEP_NARROW, mpAIField)) {
        mpEvaluatorFinal = DeepNarrowRemoverEvaluation::instance();
    }
#endif
#ifdef SITUATION_TALL_BLOCKS
    else if (getDecisionFeature(DF_BLOCK_SCAN, mpAIField)) {
        mpEvaluatorFinal = BlockRemoverEvaluation::instance();
    }
#endif
#ifdef SITUATION_TERIS
    else if (getDecisionFeature(DF_CREATING_TETRIS, mpAIField)) {
        if (getDecisionFeature(DF_REMOVE_TETRIS, mpAIField)) {
            mpEvaluatorFinal = TetrisEliminationEvaluation::instance();
        } else {
            mpEvaluatorFinal = TetrisPreparingEvaluation::instance();
        }
    }
#endif
    else {
        mpEvaluatorFinal = BaseEvaluation::instance();
    }

#ifdef DIFFERENCE_EVALUATION_
    mpEvaluatorPre  = mpEvaluatorFinal;
#endif

#ifdef SECOND_EVALUATION
    mpEvaluatorFirst = BaseEvaluation::instance();
#endif

#else
#ifdef NBS_EVALUATION
    mpEvaluatorFirst = 0;
    mpEvaluatorPre   = 0;
    mpEvaluatorFinal = NBSEvaluation::instance();
#endif
#endif
    //println(mpEvaluatorFinal->evaluationName());
}

/* Phase IV  - States Evaluation                -------------------*/
void KBlocksAIPlayer::generateGoalAction()
{
    double current_value = 0;
    if (mpEvaluatorPre != 0) {
        current_value = mpEvaluatorPre->evaluate(mpAIField);
    }

    double best_value = 0;
    Result_Sequence result = Result_Sequence(0);
    std::vector<int> best_goals = std::vector<int>(0);

    mBestRotation = 0;
    mBestPosition = 0;

    // FIRST STEP - ANALYZE FINAL BOARD_STATE
    if (mpEvaluatorFinal != 0) {
        bool firstValue = true;

        for (int i = 0; i < mNextCount; i++) {
            KBlocksField *field = new KBlocksField(mpAIField->getWidth(), mpAIField->getHeight());
            mpPlanner->getNextBoardStatus(i, field);

            if (!field) {
                continue;
            }

            KBlocksPiece *piece = new KBlocksPiece();
            mpPlanner->getNextPieceState(i, piece);

            double value = 0;
#ifndef SPECIAL_EVALUATION
            value = mpEvaluatorFinal->evaluate(field) - current_value;
#else
#ifdef NBS_EVALUATION
            SpecialEvaluationInterface *evaluator =
                dynamic_cast<SpecialEvaluationInterface *>(mpEvaluatorFinal);
            evaluator->setCurrentPiece(piece);
            value = evaluator->evaluate(field);
#endif
#endif
            result.push_back(Result(KBlocksPiece(piece), value));
#ifdef MAX_UTILITY
            bool better = (best_value < value);
#else
            bool better = (best_value > value);
#endif
            if (better || firstValue) {
                best_goals.clear();
                best_value = value;
                firstValue = false;
            }

            // alternative found
            if (best_value == value) {
                best_goals.push_back(i);
            }

            delete piece;
            if (field) {
                delete field;
            }
        }
    }

    // NO SOLUTION FOUND
    if (best_goals.empty()) {
        return ;
    }

    //SECOND STEP
#ifdef TWO_PIECE_PLANNER
#ifdef SECOND_EVALUATION
    if (mpEvaluatorFirst != 0) {
        std::vector<int> _best_goals = std::vector<int>(0);
        double _best_value = 0;
        double firstValue = true;

        KBlocksAIPlannerExtend *_planner = (dynamic_cast<KBlocksAIPlannerExtend *>(mpPlanner));

        //    double _current_value = current_value;
        for (int i = 0; i < best_goals.size(); i++) {
            int index = best_goals[i];

            KBlocksField *field = new KBlocksField();

            _planner->getNextBoardStatus(index, field, true);

            if (!field) {
                continue;
            }

            double value = mpEvaluatorFirst->evaluate(field);
#ifdef MAX_UTILITY
            bool better = (_best_value < value);
#else
            bool better = (_best_value > value);
#endif
            if (better || firstValue) {
                _best_goals.clear();
                _best_value = value;
                firstValue = false;
                better = true;
            }

            // alternative found
            if (better || (_best_value == value)) {
                _best_goals.push_back(index);
            }

            if (field) {
                delete field;
            }
        }
        best_goals = std::vector<int>(_best_goals);
    }
#endif
#endif

#ifdef INITLIST
    int goal_index = best_goals[0];
#else
    int goal_index = best_goals[rand() % best_goals.size()];
#endif

    KBlocksPiece *goal_piece = &(result[goal_index].first);

    mBestRotation = goal_piece->getRotation() - mpCurPiece->getRotation();
    mBestPosition = goal_piece->getPosX() - mpCurPiece->getPosX();
}
