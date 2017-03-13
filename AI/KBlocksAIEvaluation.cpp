/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   Copyright (C) 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
***************************************************************************/
#include "KBlocksAIEvaluation.h"
#include "KBlocksAIFeature.h"
#include "KBlocksAILog.h"

/******************************************************************************
********   Aggregate  Function   **********************************************
*******************************************************************************/
struct SFeature {
    SpecialFeatureEnumeration id;
    double weight;
};
struct Feature {
    FeatureEnumeration id;
    double weight;
};

bool getDecisionFeature(const DecisionFeatureEnumeration id, KBlocksField *field)
{
    bool  result = false;

    switch (id) {
    case DF_HEIGHT_MAX:
        set_ground_line(0);
        update_board_signature(field);
        result = (getFeature(FEATURE_MAX_HEIGHT, field) > (field->getHeight() * 0.6));
        break;

    case DF_PEEKS_COUNT:
        set_ground_line(0);
        update_board_signature(field);
        result = (getFeature(FEATURE_NARROW_COUNT, field) >= 2);
        break;

    case DF_BLOCK_SCAN:
        set_ground_line(0);
        update_board_signature(field);
        break;

    case DF_DEEP_NARROW:
        set_ground_line(0);
        update_board_signature(field);
        result = ((getFeature(FEATURE_MAX_HEIGHT_DIFFERENT, field) >= 5) && (getFeature(FEATURE_NARROW_COUNT, field) == 1));
        break;

    case DF_LAYER_SCAN:
        set_ground_line(0);
        update_board_signature(field);
        result = ((getFeature(FEATURE_MAX_HEIGHT, field) >= 8) && (getFeature(FEATURE_MAX_HEIGHT_DIFFERENT, field) <= 2));
        break;

    case DF_CREATING_TETRIS:
        set_ground_line(0);
        update_board_signature(field);
        result = ((getFeature(FEATURE_HOLES_COUNT, field) <= 5) && (getFeature(FEATURE_MAX_HEIGHT, field) <= 6));
        break;

    case DF_REMOVE_TETRIS:
        set_ground_line(0);
        update_board_signature(field);
        result = (getFeature(FEATURE_NARROW_COUNT, field) == 1);
        break;

    default:
        break;
    }

    return result;
}

/*#########################################################
* SECTION ANALYSIS EVALUATION  ############################
*########################################################## */
HalfBaseEvaluation *HalfBaseEvaluation::_instance = 0;
double HalfBaseEvaluation::evaluate(KBlocksField *field)
{
    int line = getFeature(FEATURE_MAX_HEIGHT, field) - 6;
    if (line < 0) {
        line = 0;
    }
    set_ground_line(line);
    return BaseEvaluation::instance()->evaluate(field);
}
/*#########################################################
* BASIC  EVALUATION  ######################################
*########################################################## */
BaseEvaluation *BaseEvaluation::_instance = 0;
double BaseEvaluation::evaluate(KBlocksField *field)
{
    double res = 0;
    Feature features [] = {
        {FEATURE_MAX_HEIGHT, -0.05},
        {FEATURE_HOLES_COUNT, -2.25},
        {FEATURE_WEIGHTED_BLOCKS_COUNT, -0.005},
        {FEATURE_MAX_KONTUR_LENGTH, 0.5},
        {FEATURE_MAX_HEIGHT_DIFFERENT, -1},
        {FEATURE_AVERAGE_HEIGHT_DIFFERENT, -1},
        {FEATURE_BLOCKS_OVER_HOLES_COUNT, -0.01},
        {FEATURE_WELLS_COUNT, -0.01},
        {FEATURE_PREDICTION_COUNT, 0.01},
        {FEATURE_NARROW_COUNT, -5},
    };
    /*
    Feature features [] =
    {
        {FEATURE_MAX_HEIGHT,-1.8},
        {FEATURE_HOLES_COUNT,-2.25 },
        {FEATURE_BLOCKS_COUNT,-0.5},
        {FEATURE_AVERAGE_HEIGHT,-1},
    };
    */
    /*
    Feature features [] =
    {
        {FEATURE_MAX_HEIGHT,-1.5},
        {FEATURE_HOLES_COUNT,-3.25 },
        {FEATURE_BLOCKS_COUNT,-0.005},
        {FEATURE_AVERAGE_HEIGHT,-0.7},
    };
    */
    /*
    Feature features [] =
    {
        {FEATURE_MAX_HEIGHT,-2.5},
        {FEATURE_HOLES_COUNT,-1.25 },
        {FEATURE_BLOCKS_COUNT,-0.005},
        {FEATURE_AVERAGE_HEIGHT,-1},
    };
    */
    /*

    Feature features [] =
    {
        {FEATURE_MAX_HEIGHT,-1.5},
        {FEATURE_HOLES_COUNT,-3.25 },
        {FEATURE_BLOCKS_COUNT,-0.005},
        {FEATURE_AVERAGE_HEIGHT,-0.7},
        {FEATURE_NARROW_COUNT,-20},
    };
    */
    /*
    Feature features [] =
    {
        {FEATURE_MAX_HEIGHT,-1.5},
        {FEATURE_HOLES_COUNT,-3.25 },
        {FEATURE_BLOCKS_COUNT,-0.005},
        {FEATURE_AVERAGE_HEIGHT,-0.7},
        {FEATURE_NARROW_COUNT,-20},
        {FEATURE_MAX_HEIGHT_DIFFERENT,-3},
    };
    */
    /*
    Feature features [] =
    {
        {FEATURE_MAX_HEIGHT,-1.5},
        {FEATURE_HOLES_COUNT,-3.25 },
        {FEATURE_BLOCKS_COUNT,-0.005},
        {FEATURE_AVERAGE_HEIGHT,-0.7},
        {FEATURE_NARROW_COUNT,-20},
        {FEATURE_MAX_HEIGHT_DIFFERENT,-3},
        {FEATURE_PREDICTION_COUNT,0.005},
    };
    */
    unsigned int n = sizeof(features) / sizeof(Feature);

    update_board_signature(field);

    for (unsigned int i = 0; i < n; i++) {
        res += getFeature(features[i].id, field) * features[i].weight;
    }

    return res;
}

/*#########################################################
* WELLS  STOPPER ##########################################
*########################################################## */
WellsFillerEvaluation *WellsFillerEvaluation::_instance = 0;
double WellsFillerEvaluation::evaluate(KBlocksField *field)
{
    double res = 0;

    Feature features [] = {
        {FEATURE_MAX_KONTUR_LENGTH, 1.0},
        {FEATURE_NARROW_COUNT, -1.0},
        {FEATURE_AVERAGE_HEIGHT_DIFFERENT, -1.1},
    };

    unsigned int n = sizeof(features) / sizeof(Feature);

    update_board_signature(field);

    for (unsigned int i = 0; i < n; i++) {
        res += getFeature(features[i].id, field) * features[i].weight;
    }

    return res;
}

/*#########################################################
* HEIGHT  ELIMINIATOR #####################################
*########################################################## */
HeightKillerEvaluation *HeightKillerEvaluation::_instance = 0;
double HeightKillerEvaluation::evaluate(KBlocksField *field)
{
    double res = 0;

    Feature features [] = {
        {FEATURE_MAX_HEIGHT, -2.05},
        {FEATURE_HOLES_COUNT, -1.95},
        {FEATURE_WEIGHTED_BLOCKS_COUNT, -0.05},
        {FEATURE_MAX_HEIGHT_DIFFERENT, -1.0},
        {FEATURE_AVERAGE_HEIGHT_DIFFERENT, -1},
        {FEATURE_WELLS_COUNT, -0.01},
        {FEATURE_PREDICTION_COUNT, 0.001},
    };

    unsigned int n = sizeof(features) / sizeof(Feature);

    update_board_signature(field);

    for (unsigned int i = 0; i < n; i++) {
        res += getFeature(features[i].id, field) * features[i].weight;
    }

    return res;
}

/*#########################################################
* BLOCK  ELIMINATION  #####################################
*########################################################## */
BlockRemoverEvaluation *BlockRemoverEvaluation::_instance = 0;
double BlockRemoverEvaluation::evaluate(KBlocksField *field)
{
    double res = 0;

    // ### reenable the two following #if 0 comments when the features array is not empty
#if 0
    Feature features [] = {};

    unsigned int n = sizeof(features) / sizeof(Feature);
#endif

    update_board_signature(field);

#if 0
    for (unsigned int i = 0; i < n; i++) {
        res += getFeature(features[i].id, field) * features[i].weight;
    }
#endif

    return res;
}

/*#########################################################
* DEEP NARROW ELIMINATION  ################################
*########################################################## */
DeepNarrowRemoverEvaluation *DeepNarrowRemoverEvaluation::_instance = 0;
double DeepNarrowRemoverEvaluation::evaluate(KBlocksField *field)
{
    double res = 0;

    Feature features [] = {
        {FEATURE_MAX_HEIGHT, -1.05},
        {FEATURE_WEIGHTED_BLOCKS_COUNT, -0.15},
        {FEATURE_MAX_KONTUR_LENGTH, 0.5},
        {FEATURE_MAX_HEIGHT_DIFFERENT, -1},
        {FEATURE_AVERAGE_HEIGHT_DIFFERENT, -1},
        {FEATURE_PREDICTION_COUNT, 0.001},
    };

    unsigned int n = sizeof(features) / sizeof(Feature);

    update_board_signature(field);

    for (unsigned int i = 0; i < n; i++) {
        res += getFeature(features[i].id, field) * features[i].weight;
    }

    return res;
}

/*#########################################################
* TETRIS  CREATOR  ########################################
*########################################################## */
TetrisPreparingEvaluation *TetrisPreparingEvaluation::_instance = 0;
double TetrisPreparingEvaluation::evaluate(KBlocksField *field)
{
    double res = 0;

    Feature features [] = {
        {FEATURE_MAX_HEIGHT, -0.15},
        {FEATURE_HOLES_COUNT, -3.95},
        {FEATURE_MAX_KONTUR_LENGTH, 1.5},
        {FEATURE_AVERAGE_HEIGHT_DIFFERENT, -1.75},
        {FEATURE_NARROW_COUNT, 0.2},
        {FEATURE_PREDICTION_COUNT, 0.002},
    };

    unsigned int n = sizeof(features) / sizeof(Feature);

    update_board_signature(field);

    for (unsigned int i = 0; i < n; i++) {
        res += getFeature(features[i].id, field) * features[i].weight;
    }

    return res;
}

/*#########################################################
* TETRIS  REMOVER  ########################################
*########################################################## */
TetrisEliminationEvaluation *TetrisEliminationEvaluation::_instance = 0;
double TetrisEliminationEvaluation::evaluate(KBlocksField *field)
{
    double res = 0;

    Feature features [] = {
        {FEATURE_MAX_HEIGHT, -0.75},
        {FEATURE_BLOCKS_COUNT, -2.00},
        {FEATURE_HOLES_COUNT, -1.25},
        {FEATURE_MAX_HEIGHT_DIFFERENT, -1},
        {FEATURE_AVERAGE_HEIGHT_DIFFERENT, -1},
        {FEATURE_NARROW_COUNT, -0.5},
        {FEATURE_PREDICTION_COUNT, 0.002},
    };

    unsigned int n = sizeof(features) / sizeof(Feature);

    update_board_signature(field);

    for (unsigned int i = 0; i < n; i++) {
        res += getFeature(features[i].id, field) * features[i].weight;
    }

    return res;
}

/*#########################################################
* NBS EVALUATION  #########################################
*########################################################## */
NBSEvaluation *NBSEvaluation::_instance = 0;
double NBSEvaluation::evaluate(KBlocksField *field)
{
    Feature features [] = {
        {FEATURE_MAX_HEIGHT, -62709},           //1
        {FEATURE_HOLES_COUNT, -30271},           //2
        {FEATURE_CLOSED_HOLES_COUNT, 0},           //3
        {FEATURE_MAX_HEIGHT_DIFFERENT, 35395},   //5
        {FEATURE_MAX_WELL_DEPTH, -12},           //6
        {FEATURE_WELLS_COUNT, -43810},           //7
        {FEATURE_BLOCKS_COUNT, 0},               //9
        {FEATURE_WEIGHTED_BLOCKS_COUNT, -4041},   //10
        {FEATURE_ROW_TRANSITION_COUNT, -44262},   //11
        {FEATURE_COLUMN_TRANSITION_COUNT, -5832}, //12
    };

    SFeature sfeatures [] = {
        {FEATURE_REMOVE_LINES, 13344},   //4
        {FEATURE_LANDING_HEIGHT, 0},       //8
    };

    unsigned int n1 = sizeof(features) / sizeof(Feature);

    unsigned int n2 = sizeof(sfeatures) / sizeof(SFeature);

    update_board_signature(field);

    double res = 0;

    for (unsigned int i = 0; i < n1; i++) {
        res += getFeature(features[i].id, field) * features[i].weight;
    }

    for (unsigned int i = 0; i < n2; i++) {
        res += getSpecialFeature(sfeatures[i].id, field, mpField, mpPiece) * sfeatures[i].weight;
    }

    return res;
}
