/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 University Freiburg                                *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef EVALUATIONINTERFACE_H
#define EVALUATIONINTERFACE_H

#include "../KBlocksField.h"
#include "../KBlocksPiece.h"

#define DEBUG_EVALUATION
#ifdef  DEBUG_EVALUATION
#define CLASSNAME(class)\
    const char* evaluationName()\
    {\
        return #class;\
    }
#else
#define CLASSNAME(class) 
#endif 

class EvaluationInterface
{
public:  
    EvaluationInterface(){};
    virtual ~EvaluationInterface(){};
    virtual double evaluate(KBlocksField*) = 0;
#ifdef DEBUG_EVALUATION
    virtual const char* evaluationName()
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
        mpPiece = 0;
        mpField = 0;
    }
    virtual ~SpecialEvaluationInterface(){};
    
    virtual double evaluate(KBlocksField*) = 0;
    void setCurrentPiece(KBlocksPiece * piece)
    {
        mpPiece = piece;
    }
    void setCurrentBoard(KBlocksField * field)
    {
        mpField = field;
    }

#ifdef DEBUG_EVALUATION
    virtual const char* evaluationName()
    {
        return "";
    }
#endif
    
protected:
    KBlocksPiece* mpPiece;
    KBlocksField* mpField;
};

#endif //EVALUATIONINTERFACE_H