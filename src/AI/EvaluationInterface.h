/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   SPDX-FileCopyrightText: 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#ifndef EVALUATIONINTERFACE_H
#define EVALUATIONINTERFACE_H

#include "../KBlocksField.h"
#include "../KBlocksPiece.h"

#define DEBUG_EVALUATION
#ifdef  DEBUG_EVALUATION
#define CLASSNAME(class)\
    const char* evaluationName()\
    override {\
        return #class;\
    }
#else
#define CLASSNAME(class)
#endif

class EvaluationInterface
{
public:
    EvaluationInterface() {};
    virtual ~EvaluationInterface() {};
    virtual double evaluate(KBlocksField *) = 0;
#ifdef DEBUG_EVALUATION
    virtual const char *evaluationName()
    {
        return "";
    }
#endif
};

class SpecialEvaluationInterface : public EvaluationInterface
{
public:
    SpecialEvaluationInterface()
    {
        mpPiece = nullptr;
        mpField = nullptr;
    }
    ~SpecialEvaluationInterface() override {};

    double evaluate(KBlocksField *) override = 0;
    void setCurrentPiece(KBlocksPiece *piece)
    {
        mpPiece = piece;
    }
    void setCurrentBoard(KBlocksField *field)
    {
        mpField = field;
    }

#ifdef DEBUG_EVALUATION
    const char *evaluationName() override
    {
        return "";
    }
#endif

protected:
    KBlocksPiece *mpPiece;
    KBlocksField *mpField;
};

#endif //EVALUATIONINTERFACE_H
