/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2010 University Freiburg                                *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSAIEVALUATION_H
#define KBLOCKSAIEVALUATION_H

#include "KBlocksAITypeDefine.h"

#include "EvaluationInterface.h"

#include "../KBlocksField.h"

enum DecisionFeatureEnumeration
{  
    DF_HEIGHT_MAX,
    DF_PEEKS_COUNT,
    DF_BLOCK_SCAN,
    DF_LAYER_SCAN,
    DF_CREATING_TETRIS,
    DF_REMOVE_TETRIS,
    DF_DEEP_NARROW,
};

bool getDecisionFeature(const DecisionFeatureEnumeration, KBlocksField*);

/******************************************************
****   Evaluation Function    *************************
*******************************************************/
class BaseEvaluation : public EvaluationInterface
{
public:
    double evaluate(KBlocksField*);
    static BaseEvaluation *instance()
    {
        if ( _instance == 0 )
        {
            _instance = new BaseEvaluation();
        }
        return _instance;
    }
    CLASSNAME(BaseEvaluation)
    
private:
    BaseEvaluation(){};
    ~BaseEvaluation(){};
    static BaseEvaluation *_instance;
};
/*###################################################*/
class HalfBaseEvaluation : public EvaluationInterface
{
public:
    double evaluate(KBlocksField*);
    static HalfBaseEvaluation *instance()
    {
        if ( _instance == 0 )
        {
            _instance = new HalfBaseEvaluation();
        }
        return _instance;
    }
    CLASSNAME(HalfBaseEvaluation)
    
private:
    HalfBaseEvaluation(){};
    ~HalfBaseEvaluation(){};
    static HalfBaseEvaluation *_instance;
};
/*###################################################*/
class WellsFillerEvaluation : public EvaluationInterface
{
public:
    double evaluate(KBlocksField*);
    static WellsFillerEvaluation *instance()
    {
        if ( _instance == 0 )
        {
            _instance = new WellsFillerEvaluation();
        }
        return _instance;
    }
    CLASSNAME(WellsFillerEvaluation)
    
private:
    WellsFillerEvaluation(){};
    ~WellsFillerEvaluation(){};
    static WellsFillerEvaluation *_instance;
};
/*###################################################*/
class HeightKillerEvaluation : public EvaluationInterface
{
public:
    double evaluate(KBlocksField*);
    static HeightKillerEvaluation *instance()
    {
        if ( _instance == 0 )
        {
            _instance = new HeightKillerEvaluation();
        }
        return _instance;
    }
    CLASSNAME(HeightKillerEvaluation)
    
private:
    HeightKillerEvaluation(){};
    ~HeightKillerEvaluation(){};
    static HeightKillerEvaluation *_instance;
};
/*###################################################*/
class BlockRemoverEvaluation : public EvaluationInterface
{
public:
    double evaluate(KBlocksField*);
    static BlockRemoverEvaluation *instance()
    {
        if ( _instance == 0 )
        {
            _instance = new BlockRemoverEvaluation();
        }
        return _instance;
    }
    CLASSNAME(BlockRemoverEvaluation)
    
private:
    BlockRemoverEvaluation(){};
    ~BlockRemoverEvaluation(){};
    static BlockRemoverEvaluation *_instance;
};
/*###################################################*/
class DeepNarrowRemoverEvaluation : public EvaluationInterface
{
public:
    double evaluate(KBlocksField*);
    static DeepNarrowRemoverEvaluation *instance()
    {
        if ( _instance == 0 )
        {
            _instance = new DeepNarrowRemoverEvaluation();
        }
        return _instance;
    }
    CLASSNAME(DeepNarrowRemoverEvaluation)
    
private:
    DeepNarrowRemoverEvaluation(){};
    ~DeepNarrowRemoverEvaluation(){};
    static DeepNarrowRemoverEvaluation *_instance;
};
/*###################################################*/
class TetrisPreparingEvaluation : public EvaluationInterface
{
public:
    double evaluate(KBlocksField*);
    static TetrisPreparingEvaluation *instance()
    {
        if ( _instance == 0 )
        {
            _instance = new TetrisPreparingEvaluation();
        }
        return _instance;
    }
    CLASSNAME(TetrisPreparingEvaluation)
    
private:
    TetrisPreparingEvaluation(){};
    ~TetrisPreparingEvaluation(){};
    static TetrisPreparingEvaluation *_instance;
};
/*###################################################*/
class TetrisEliminationEvaluation : public EvaluationInterface
{
public:  
    double evaluate(KBlocksField*);
    static TetrisEliminationEvaluation *instance()
    {
        if ( _instance == 0 )
        {
            _instance = new TetrisEliminationEvaluation();
        }
        return _instance;
    }
    CLASSNAME(TetrisEliminationEvaluation)
    
private:
    TetrisEliminationEvaluation(){};
    ~TetrisEliminationEvaluation(){};
    static TetrisEliminationEvaluation *_instance;
};
/*###################################################*/
class NBSEvaluation : public SpecialEvaluationInterface
{
public:
    double evaluate(KBlocksField*);
    static NBSEvaluation *instance()
    {
        if ( _instance == 0 )
        {
            _instance = new NBSEvaluation();
        }
        return _instance;
    }
    CLASSNAME(NBSEvaluation)
    
private:
    NBSEvaluation(){};
    ~NBSEvaluation(){};
    static NBSEvaluation *_instance;
};
/*###################################################*/

#endif //KBLOCKSAIEVALUATION_H
