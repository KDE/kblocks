/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   Copyright (C) 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
***************************************************************************/
#include "KBlocksAIFeature.h"

#include <vector>

//#define ground_line 0
static int ground_line = 0;
static std::vector<int> board_signature = std::vector<int> (0);

/************************************************************************************
****  Util Function   ***************************************************************
*************************************************************************************/
int Abs(int a)
{
    if (a < 0) {
        return -a;
    }
    return a;
}

double Abs(double a)
{
    if (a < 0) {
        return -a;
    }
    return a;
}

int Min(int a, int b)
{
    if (a > b) {
        return b;
    }
    return a;
}

int Max(int a, int b)
{
    if (a < b) {
        return b;
    }
    return a;
}

double Min(double a, double b)
{
    if (a > b) {
        return b;
    }
    return a;
}

double Max(double a, double b)
{
    if (a < b) {
        return b;
    }
    return a;
}
//----------------------------------------------------------------
// returns the number of row -last non-empty cell- plus one
int column_max_height(int column, KBlocksField *field)
{
    int width = field->getWidth();
    int height = field->getHeight();

    if ((column < 0) || (column >= width)) {
        //printf("WARNING: invalid column index %d",column);
    }

    for (int j = 0; j < height; j++) {
        if (field->getCell(column, j)) {
            return j;
        }
    }
    return height;
}
//----------------------------------------------------------------
void set_ground_line(int null_line)
{
    ground_line = null_line;
}
//----------------------------------------------------------------
void update_board_signature(KBlocksField *field)
{
    int width = field->getWidth();
    if (board_signature.empty()) {
        board_signature = std::vector<int> (width);
    }
    for (int i = 0; i <  width; i++) {
        board_signature[i] = column_max_height(i, field);
    }
}
/******************************************************************************
********   Feature  List   ****************************************************
*******************************************************************************/
double getFeature(const FeatureEnumeration id, KBlocksField *field)
{
    EvaluationInterface *e = 0;
    switch (id) {
    case FEATURE_BLOCKS_COUNT:
        e = Evaluation_Blocks_Count::instance(); break;
    case FEATURE_HOLES_COUNT:
        e = Evaluation_Holes_Count::instance(); break;
    case FEATURE_MAX_HEIGHT:
        e = Evaluation_Max_Height::instance(); break;
    case FEATURE_AVERAGE_HEIGHT:
        e = Evaluation_Average_Height::instance(); break;
    case FEATURE_AVERAGE_HEIGHT_DIFFERENT:
        e = Evaluation_Average_Height_Difference::instance(); break;
    case FEATURE_MAX_HEIGHT_DIFFERENT:
        e = Evaluation_Max_Height_Difference::instance(); break;
    case FEATURE_BLOCKS_OVER_HOLES_COUNT:
        e = Evaluation_Blocks_Over_Holes_Count::instance(); break;
    case FEATURE_KONTUR_COUNT:
        e = Evaluation_Kontur_Count::instance(); break;
    case FEATURE_MAX_KONTUR_LENGTH:
        e = Evaluation_Max_Kontur_Length::instance(); break;
    case FEATURE_NARROW_COUNT:
        e = Evaluation_Narrow_Count::instance(); break;
    case FEATURE_PREDICTION_COUNT:
        e = Evaluation_Prediction_Count::instance(); break;
    case FEATURE_CLOSED_HOLES_COUNT:
        e = Evaluation_Closed_Holes_Count::instance(); break;
    case FEATURE_WELLS_COUNT:
        e = Evaluation_Wells_Count::instance(); break;
    case FEATURE_WEIGHTED_BLOCKS_COUNT:
        e = Evaluation_Weighted_Blocks_Count::instance(); break;
    case FEATURE_ROW_TRANSITION_COUNT:
        e = Evaluation_Row_Transition_Count::instance(); break;
    case FEATURE_COLUMN_TRANSITION_COUNT:
        e = Evaluation_Column_Transition_Count::instance(); break;
    case FEATURE_MAX_WELL_DEPTH:
        e = Evaluation_Max_Well_Depth::instance(); break;
    default: ;
        //  printf("WARNING: Evaluation not found!");
    }
    if (e == 0) {
        return 0;
    }
    return e->evaluate(field);
}

const char *getFeatureName(const FeatureEnumeration id)
{
    switch (id) {
    case FEATURE_MAX_HEIGHT:
        return "MAX_HEIGHT";
    case FEATURE_HOLES_COUNT:
        return "HOLES_COUNT";
    case FEATURE_AVERAGE_HEIGHT:
        return "AVERAGE_HEIGHT";
    case FEATURE_AVERAGE_HEIGHT_DIFFERENT:
        return "AVERAGE_HEIGHT_DIFFERENCE";
    case FEATURE_MAX_HEIGHT_DIFFERENT:
        return "MAX_HEIGHT_DIFFERENCE";
    case FEATURE_BLOCKS_OVER_HOLES_COUNT:
        return "BLOCKS_OVER_HOLES_COUNT";
    case FEATURE_KONTUR_COUNT:
        return "KONTUR_COUNT";
    case FEATURE_MAX_KONTUR_LENGTH:
        return "MAX_KONTUR_LENGTH";
    case FEATURE_CLOSED_HOLES_COUNT:
        return "CLOSED_HOLES_COUNT";
    case FEATURE_WELLS_COUNT:
        return "WELLS_COUNT";
    case FEATURE_BLOCKS_COUNT:
        return "BLOCKS_COUNT";
    case FEATURE_WEIGHTED_BLOCKS_COUNT:
        return "WEIGHTED_BLOCKS_COUNT";
    case FEATURE_ROW_TRANSITION_COUNT:
        return "ROW_TRANSITION_COUNT";
    case FEATURE_COLUMN_TRANSITION_COUNT:
        return "COLUMN_TRANSITION_COUNT";
    case FEATURE_MAX_WELL_DEPTH:
        return "MAX_WELL_DEPTH";
    case FEATURE_NARROW_COUNT:
        return "NARROW_COUNT";
    case FEATURE_PREDICTION_COUNT:
        return "PREDICTION_COUNT";
    default: ;
        //  printf("WARNING: Evaluation not found!");
    }
    return "";
}
//----------------------------------------------------------------
double getSpecialFeature(const SpecialFeatureEnumeration id, KBlocksField *newField, KBlocksField *oldField, KBlocksPiece *piece)
{
    SpecialEvaluationInterface *e = 0;
    switch (id) {
    case FEATURE_REMOVE_LINES:
        e = Evaluation_Remove_Lines::instance(); break;
    case FEATURE_LANDING_HEIGHT:
        e = Evaluation_Landing_Height::instance(); break;
    default:;
    }

    if (e == 0) {
        return 0;
    }

    e->setCurrentPiece(piece);
    e->setCurrentBoard(oldField);

    return e->evaluate(newField);
}
/******************************************************************************
********   Primitiv Function   ==   Feature   *********************************
*******************************************************************************/
Evaluation_Max_Height *Evaluation_Max_Height::_instance = 0;
double Evaluation_Max_Height::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    int max = 0;
    for (int i = 0; i < w; i++) {
        max = Max(max, board_signature[i]);
    }
    return max;
}
//----------------------------------------------------------------
Evaluation_Holes_Count *Evaluation_Holes_Count::_instance = 0;
double Evaluation_Holes_Count::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    int h = field->getHeight();
    int count = 0;
    for (int i = 0; i < w; i++) {
        unsigned int maxheight = board_signature[i];
        for (int j = maxheight + 1;  j < (h - ground_line); j++) {
            if (!field->getCell(i, j)) {
                count++;
            }
        }
    }
    return count;
}
//----------------------------------------------------------------
Evaluation_Average_Height *Evaluation_Average_Height::_instance = 0;
double Evaluation_Average_Height::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    double average = 0;
    for (int i = 0; i < w; i++) {
        average += board_signature[i];
    }
    return average / w;
}
//----------------------------------------------------------------
Evaluation_Average_Height_Difference *Evaluation_Average_Height_Difference::_instance = 0;
double Evaluation_Average_Height_Difference::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    double average = 0;
    int h0 = board_signature[0];
    for (int i = 1; i < w; i++) {
        int h1 = board_signature[i];
        int dh = Abs(h1 - h0);
        h0 = h1;
        average += dh;
    }
    return average / (w - 1);
}
//----------------------------------------------------------------
Evaluation_Max_Height_Difference *Evaluation_Max_Height_Difference::_instance = 0;
double Evaluation_Max_Height_Difference::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    int max = 0;
    int h0 = board_signature[0];
    for (int i = 1; i < w; i++) {
        int h1 = board_signature[i];
        int dh = Abs(h1 - h0);
        h0 = h1;
        max = Max(max, dh);
    }
    return max;
}
//----------------------------------------------------------------
Evaluation_Kontur_Count *Evaluation_Kontur_Count::_instance = 0;
double Evaluation_Kontur_Count::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    int curH = board_signature[0];
    double count = 0;
    for (int i = 1; i < w; i++) {
        int newH = board_signature[i];
        if (newH == curH) {
            count++;
        }
        curH = newH;
    }
    return count;
}
//----------------------------------------------------------------
Evaluation_Max_Kontur_Length *Evaluation_Max_Kontur_Length::_instance = 0;
double Evaluation_Max_Kontur_Length::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    int curH = board_signature[0];
    double count = 0;
    double max = 0;
    for (int i = 1; i < w; i++) {
        int newH = board_signature[i];
        if (newH == curH) {
            count++;
        } else {
            count = 0;
        }
        max = Max(max, count);
        curH = newH;
    }
    return max;
}
//----------------------------------------------------------------
Evaluation_Closed_Holes_Count *Evaluation_Closed_Holes_Count::_instance = 0;
double Evaluation_Closed_Holes_Count::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    int h = field->getHeight();
    int global_count = 0;
    for (int col = 0; col < w; col++) {
        int local_count = 0;
        for (int row = board_signature[col]; row < (h - ground_line); row++) {
            bool cell = field->getCell(col, row); // TODO : ?? Does not make sense...
            if (cell) {
                local_count++;
            } else {
                global_count += local_count;
                local_count = 0;
            }
        }
        global_count += local_count;
    }
    return global_count;
}
//----------------------------------------------------------------
Evaluation_Blocks_Count *Evaluation_Blocks_Count::_instance = 0;
double Evaluation_Blocks_Count::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    int h = field->getHeight();
    int count = 0;
    for (int j = 0; j < (h - ground_line); j++) {
        for (int i = 0; i < w; i++) {
            if (field->getCell(i, j)) {
                count++;
            }
        }
    }
    return count;
}
//----------------------------------------------------------------
Evaluation_Blocks_Over_Holes_Count *Evaluation_Blocks_Over_Holes_Count::_instance = 0;
double Evaluation_Blocks_Over_Holes_Count::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    int h0 = field->getHeight();
    int count = 0;
    for (int col = 0; col < w; col++) {
        int h1 = column_max_height(col, field);
        for (int row = h1; row < (h0 - ground_line); row++) {
            bool cell = field->getCell(col, row);
            if (cell) {
                count++;
            } else {
                break;
            }
        }
    }
    return count;
}
//----------------------------------------------------------------
Evaluation_Weighted_Blocks_Count *Evaluation_Weighted_Blocks_Count::_instance = 0;
double Evaluation_Weighted_Blocks_Count::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    int h = field->getHeight();
    int count = 0;
    for (int j = 0; j < (h - ground_line); j++) {
        for (int i = 0; i < w; i++) {
            if (field->getCell(i, j)) {
                count += (h - ground_line - j); // TODO : Jet wrote (h - j) here, which I don't understand
            }
        }
    }
    return count;
}
//----------------------------------------------------------------
Evaluation_Row_Transition_Count *Evaluation_Row_Transition_Count::_instance = 0;
double Evaluation_Row_Transition_Count::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    int h = field->getHeight();
    int count = 0;
    for (int j = 0; j < (h - ground_line); j++) {
        bool lastCell = true;
        for (int i = 0; i < w; i++) {
            bool cell = field->getCell(i, j);
            if ((cell != lastCell) || ((i + 1) == w)) {
                count++;
            }
            lastCell = cell;
        }
    }
    return count;
}
//----------------------------------------------------------------
Evaluation_Column_Transition_Count *Evaluation_Column_Transition_Count::_instance = 0;
double Evaluation_Column_Transition_Count::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    int h = field->getHeight();
    int count = 0;
    for (int i = 0; i < w; i++) {
        bool lastCell = true;
        for (int j = 0; j < (h - ground_line); j++) {
            bool cell = field->getCell(i, j);
            if ((cell != lastCell) || ((j + 1) == (h - ground_line))) { // TODO : why (j+1)==(h - ground_line)??
                count++;
            }
            lastCell = cell;
        }
    }
    return count;
}
//----------------------------------------------------------------
Evaluation_Narrow_Count *Evaluation_Narrow_Count::_instance = 0;
double Evaluation_Narrow_Count::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    const int H = 5;
    int lh = 0;
    int ch = board_signature[0];
    int rh = board_signature[1];
    int count = 0;
    if ((ch < rh) && (Abs(ch - rh) >= H)) {
        count++;
    }
    for (int i = 2; i < w; i++) {
        lh = ch;
        ch = rh;
        rh = column_max_height(i, field);
        int ldh = Abs(ch - lh);
        int rdh = Abs(ch - rh);
        if ((ldh >= H) && (rdh >= H)) {
            count++;
        }
    }
    if ((ch > rh) && (Abs(ch - rh) >= H)) {
        count++;
    }
    return count;
}
//----------------------------------------------------------------
Evaluation_Wells_Count *Evaluation_Wells_Count::_instance = 0;
double Evaluation_Wells_Count::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    int lh = 0;
    int ch = board_signature[0];
    int rh = board_signature[1];
    int count = 0;
    if (ch < rh) {
        count += (rh - ch);
    }
    for (int i = 2; i < w; i++) {
        lh = ch;
        ch = rh;
        rh = column_max_height(i, field);
        if ((lh > ch) && (rh > ch)) {
            count += Min(lh - ch, rh - ch);
        }
    }
    if (ch > rh) {
        count += (ch - rh);
    }
    return count;
}
//----------------------------------------------------------------
Evaluation_Max_Well_Depth *Evaluation_Max_Well_Depth::_instance = 0;
double Evaluation_Max_Well_Depth::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    int lh = 0;
    int ch = board_signature[0];
    int rh = board_signature[1];
    int max = 0;
    if (ch < rh) {
        max = Max(max, rh - ch);
    }
    for (int i = 2; i < w; i++) {
        lh = ch;
        ch = rh;
        rh = column_max_height(i, field);
        if ((lh > ch) && (rh > ch)) {
            max = Max(max, Min(lh - ch, rh - ch));
        }
    }
    if (ch > rh) {
        max = Max(max, ch - rh);
    }
    return max;
}
//----------------------------------------------------------------
Evaluation_Prediction_Count *Evaluation_Prediction_Count::_instance = 0;
double Evaluation_Prediction_Count::evaluate(KBlocksField *field)
{
    int w = field->getWidth();
    int count = 0;
    KBlocksPiece piece;
    int signatureCount = w;
    int *signaturePiece = new int[w];
    for (int type = 0; type < PieceType_Detail_Max_Count; type++) {
        piece.fromValue(type);
        int pw = piece.getWidth();
        for (int px = 0; px < w - pw + 1; px++) {
            signatureCount = piece.getSignature(signaturePiece);

            bool matchedAll = true;
            int matchedValue = board_signature[px] + signaturePiece[0];
            for (int i = 0; i < signatureCount; i++) {
                if (matchedValue != board_signature[px + i] + signaturePiece[i]) {
                    matchedAll = false;
                    break;
                }
            }
            if (matchedAll) {
                count++;
            }
        }
    }
    delete[] signaturePiece;
    return count;
}
//# SPECIAL FEATURE #######################################
Evaluation_Landing_Height *Evaluation_Landing_Height::_instance = 0;
double Evaluation_Landing_Height::evaluate(KBlocksField *field)
{
    if (mpPiece == 0) {
        return 0;
    }
    return field->getHeight() - mpPiece->getPosY();
}
//----------------------------------------------------------------
Evaluation_Remove_Lines *Evaluation_Remove_Lines::_instance = 0;
double Evaluation_Remove_Lines::evaluate(KBlocksField *field)
{
    if (mpField == 0) {
        return 0;
    }
    int cblock = getFeature(FEATURE_BLOCKS_COUNT, mpField);
    int nblock = getFeature(FEATURE_BLOCKS_COUNT, field);
    if (cblock > nblock) {
        return ((cblock - (nblock - 4)) / 10);
    }
    return 0;
}
