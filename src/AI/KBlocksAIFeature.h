/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   SPDX-FileCopyrightText: 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#ifndef KBLOCKSAIFEATURE_H
#define KBLOCKSAIFEATURE_H

#include "KBlocksAITypeDefine.h"
#include "EvaluationInterface.h"

#include "../KBlocksField.h"
#include "../KBlocksPiece.h"

/*******************************************************
****   Feature  List   ********************************
*******************************************************/

enum FeatureEnumeration {
    FEATURE_MAX_HEIGHT,
    FEATURE_HOLES_COUNT,
    FEATURE_CLOSED_HOLES_COUNT,
    FEATURE_WELLS_COUNT,
    FEATURE_AVERAGE_HEIGHT,
    FEATURE_AVERAGE_HEIGHT_DIFFERENT,
    FEATURE_MAX_HEIGHT_DIFFERENT,
    FEATURE_KONTUR_COUNT,
    FEATURE_MAX_KONTUR_LENGTH,
    FEATURE_BLOCKS_COUNT,
    FEATURE_BLOCKS_OVER_HOLES_COUNT,
    FEATURE_WEIGHTED_BLOCKS_COUNT,
    FEATURE_ROW_TRANSITION_COUNT,
    FEATURE_COLUMN_TRANSITION_COUNT,
    FEATURE_MAX_WELL_DEPTH,
    FEATURE_NARROW_COUNT,
    FEATURE_PREDICTION_COUNT,
    FEATURE_COUNT
};

enum SpecialFeatureEnumeration {
    FEATURE_REMOVE_LINES,
    FEATURE_LANDING_HEIGHT
};

double getFeature(const FeatureEnumeration, KBlocksField *);
const char *getFeatureName(const FeatureEnumeration);
double getSpecialFeature(const SpecialFeatureEnumeration, KBlocksField *, KBlocksField *, KBlocksPiece *);
void update_board_signature(KBlocksField *field);
void set_ground_line(int);

/*******************************************************
****   Feature     ************************************
*******************************************************/
class Evaluation_Max_Height: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Max_Height *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Max_Height();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Max_Height)
private:
    Evaluation_Max_Height() {}
    ~Evaluation_Max_Height() override {}
    static Evaluation_Max_Height *_instance;
};
//------------------------------------------------------
class  Evaluation_Holes_Count: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Holes_Count *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Holes_Count();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Holes_Count)
private:
    Evaluation_Holes_Count() {}
    ~Evaluation_Holes_Count() override {}
    static Evaluation_Holes_Count *_instance;
};
//------------------------------------------------------
class  Evaluation_Average_Height: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Average_Height *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Average_Height();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Average_Height)
private:
    Evaluation_Average_Height() {}
    ~Evaluation_Average_Height() override {}
    static Evaluation_Average_Height *_instance;
};
//------------------------------------------------------
class  Evaluation_Max_Height_Difference: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Max_Height_Difference *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Max_Height_Difference();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Max_Height_Difference)
private:
    Evaluation_Max_Height_Difference() {}
    ~Evaluation_Max_Height_Difference() override {}
    static Evaluation_Max_Height_Difference *_instance;
};
//------------------------------------------------------
class  Evaluation_Average_Height_Difference: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Average_Height_Difference *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Average_Height_Difference();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Average_Height_Difference)
private:
    Evaluation_Average_Height_Difference() {}
    ~Evaluation_Average_Height_Difference() override {}
    static Evaluation_Average_Height_Difference *_instance;
};
//------------------------------------------------------
class  Evaluation_Kontur_Count: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Kontur_Count *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Kontur_Count();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Kontur_Count)
private:
    Evaluation_Kontur_Count() {}
    ~Evaluation_Kontur_Count() override {}
    static Evaluation_Kontur_Count *_instance;
};
//------------------------------------------------------
class  Evaluation_Max_Kontur_Length: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Max_Kontur_Length *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Max_Kontur_Length();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Max_Kontur_Length)
private:
    Evaluation_Max_Kontur_Length() {}
    ~Evaluation_Max_Kontur_Length() override {}
    static Evaluation_Max_Kontur_Length *_instance;
};
//------------------------------------------------------
class  Evaluation_Closed_Holes_Count: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Closed_Holes_Count *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Closed_Holes_Count();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Closed_Holes_Count)
private:
    Evaluation_Closed_Holes_Count() {}
    ~Evaluation_Closed_Holes_Count() override {}
    static Evaluation_Closed_Holes_Count *_instance;
};
//------------------------------------------------------
class  Evaluation_Wells_Count: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Wells_Count *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Wells_Count();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Wells_Count)
private:
    Evaluation_Wells_Count() {}
    ~Evaluation_Wells_Count() override {}
    static Evaluation_Wells_Count *_instance;
};
//------------------------------------------------------
class  Evaluation_Blocks_Count: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Blocks_Count *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Blocks_Count();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Blocks_Count)
private:
    Evaluation_Blocks_Count() {}
    ~Evaluation_Blocks_Count() override {}
    static Evaluation_Blocks_Count *_instance;
};
//------------------------------------------------------
class  Evaluation_Blocks_Over_Holes_Count: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Blocks_Over_Holes_Count *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Blocks_Over_Holes_Count();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Blocks_Over_Holes_Count)
private:
    Evaluation_Blocks_Over_Holes_Count() {}
    ~Evaluation_Blocks_Over_Holes_Count() override {}
    static Evaluation_Blocks_Over_Holes_Count *_instance;
};
//------------------------------------------------------
class Evaluation_Weighted_Blocks_Count: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Weighted_Blocks_Count *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Weighted_Blocks_Count();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Weighted_Blocks_Count)
private:
    Evaluation_Weighted_Blocks_Count() {}
    ~Evaluation_Weighted_Blocks_Count() override {}
    static Evaluation_Weighted_Blocks_Count *_instance;
};
//------------------------------------------------------
class Evaluation_Row_Transition_Count: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Row_Transition_Count *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Row_Transition_Count();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Row_Transition_Count)
private:
    Evaluation_Row_Transition_Count() {}
    ~Evaluation_Row_Transition_Count() override {}
    static Evaluation_Row_Transition_Count *_instance;
};
//------------------------------------------------------
class Evaluation_Column_Transition_Count: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Column_Transition_Count *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Column_Transition_Count();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Column_Transition_Count)
private:
    Evaluation_Column_Transition_Count() {}
    ~Evaluation_Column_Transition_Count() override {}
    static Evaluation_Column_Transition_Count *_instance;
};
//------------------------------------------------------
class Evaluation_Narrow_Count: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Narrow_Count *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Narrow_Count();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Narrow_Count)
private:
    Evaluation_Narrow_Count() {}
    ~Evaluation_Narrow_Count() override {}
    static Evaluation_Narrow_Count *_instance;
};
//------------------------------------------------------
class Evaluation_Prediction_Count: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Prediction_Count *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Prediction_Count();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Prediction_Count)
private:
    Evaluation_Prediction_Count() {}
    ~Evaluation_Prediction_Count() override {}
    static Evaluation_Prediction_Count *_instance;
};
//------------------------------------------------------
class Evaluation_Max_Well_Depth: public EvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Max_Well_Depth *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Max_Well_Depth();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Max_Well_Depth)
private:
    Evaluation_Max_Well_Depth() {}
    ~Evaluation_Max_Well_Depth() override {}
    static Evaluation_Max_Well_Depth *_instance;
};

//# SPECIAL FEATURE #######################################
class Evaluation_Remove_Lines: public SpecialEvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Remove_Lines *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Remove_Lines();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Remove_Lines)
private:
    Evaluation_Remove_Lines() {}
    ~Evaluation_Remove_Lines() override {}
    static Evaluation_Remove_Lines *_instance;
};
//------------------------------------------------------
class Evaluation_Landing_Height: public SpecialEvaluationInterface
{
public:
    double evaluate(KBlocksField *) override;
    static Evaluation_Landing_Height *instance()
    {
        if (_instance == nullptr) {
            _instance = new Evaluation_Landing_Height();
        }
        return _instance;
    }
    CLASSNAME(Evaluation_Landing_Height)
private:
    Evaluation_Landing_Height() {}
    ~Evaluation_Landing_Height() override {}
    static Evaluation_Landing_Height *_instance;
};

#endif //KBLOCKSAIFEATURE_H
