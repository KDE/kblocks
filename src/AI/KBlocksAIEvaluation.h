/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   SPDX-FileCopyrightText: 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#ifndef KBLOCKSAIEVALUATION_H
#define KBLOCKSAIEVALUATION_H

#include "KBlocksAITypeDefine.h"

#include "EvaluationInterface.h"

#include "../KBlocksField.h"

enum DecisionFeatureEnumeration {
    DF_HEIGHT_MAX,
    DF_PEEKS_COUNT,
    DF_BLOCK_SCAN,
    DF_LAYER_SCAN,
    DF_CREATING_TETRIS,
    DF_REMOVE_TETRIS,
    DF_DEEP_NARROW
};

bool getDecisionFeature(const DecisionFeatureEnumeration, KBlocksField *);

/******************************************************
****   Evaluation Function    *************************
*******************************************************/
class BaseEvaluation : public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static BaseEvaluation *instance()
    {
        if (_instance == nullptr) {
            _instance = new BaseEvaluation();
        }
        return _instance;
    }
    CLASSNAME(BaseEvaluation)

private:
    BaseEvaluation() {};
    ~BaseEvaluation() override {};
    static BaseEvaluation *_instance;
};
/*###################################################*/
class HalfBaseEvaluation : public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static HalfBaseEvaluation *instance()
    {
        if (_instance == nullptr) {
            _instance = new HalfBaseEvaluation();
        }
        return _instance;
    }
    CLASSNAME(HalfBaseEvaluation)

private:
    HalfBaseEvaluation() {};
    ~HalfBaseEvaluation() override {};
    static HalfBaseEvaluation *_instance;
};
/*###################################################*/
class WellsFillerEvaluation : public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static WellsFillerEvaluation *instance()
    {
        if (_instance == nullptr) {
            _instance = new WellsFillerEvaluation();
        }
        return _instance;
    }
    CLASSNAME(WellsFillerEvaluation)

private:
    WellsFillerEvaluation() {};
    ~WellsFillerEvaluation() override {};
    static WellsFillerEvaluation *_instance;
};
/*###################################################*/
class HeightKillerEvaluation : public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static HeightKillerEvaluation *instance()
    {
        if (_instance == nullptr) {
            _instance = new HeightKillerEvaluation();
        }
        return _instance;
    }
    CLASSNAME(HeightKillerEvaluation)

private:
    HeightKillerEvaluation() {};
    ~HeightKillerEvaluation() override {};
    static HeightKillerEvaluation *_instance;
};
/*###################################################*/
class BlockRemoverEvaluation : public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static BlockRemoverEvaluation *instance()
    {
        if (_instance == nullptr) {
            _instance = new BlockRemoverEvaluation();
        }
        return _instance;
    }
    CLASSNAME(BlockRemoverEvaluation)

private:
    BlockRemoverEvaluation() {};
    ~BlockRemoverEvaluation() override {};
    static BlockRemoverEvaluation *_instance;
};
/*###################################################*/
class DeepNarrowRemoverEvaluation : public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static DeepNarrowRemoverEvaluation *instance()
    {
        if (_instance == nullptr) {
            _instance = new DeepNarrowRemoverEvaluation();
        }
        return _instance;
    }
    CLASSNAME(DeepNarrowRemoverEvaluation)

private:
    DeepNarrowRemoverEvaluation() {};
    ~DeepNarrowRemoverEvaluation() override {};
    static DeepNarrowRemoverEvaluation *_instance;
};
/*###################################################*/
class TetrisPreparingEvaluation : public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static TetrisPreparingEvaluation *instance()
    {
        if (_instance == nullptr) {
            _instance = new TetrisPreparingEvaluation();
        }
        return _instance;
    }
    CLASSNAME(TetrisPreparingEvaluation)

private:
    TetrisPreparingEvaluation() {};
    ~TetrisPreparingEvaluation() override {};
    static TetrisPreparingEvaluation *_instance;
};
/*###################################################*/
class TetrisEliminationEvaluation : public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static TetrisEliminationEvaluation *instance()
    {
        if (_instance == nullptr) {
            _instance = new TetrisEliminationEvaluation();
        }
        return _instance;
    }
    CLASSNAME(TetrisEliminationEvaluation)

private:
    TetrisEliminationEvaluation() {};
    ~TetrisEliminationEvaluation() override {};
    static TetrisEliminationEvaluation *_instance;
};
/*###################################################*/
class NBSEvaluation : public SpecialEvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static NBSEvaluation *instance()
    {
        if (_instance == nullptr) {
            _instance = new NBSEvaluation();
        }
        return _instance;
    }
    CLASSNAME(NBSEvaluation)

private:
    NBSEvaluation() {};
    ~NBSEvaluation() override {};
    static NBSEvaluation *_instance;
};
/*###################################################*/

#endif //KBLOCKSAIEVALUATION_H
