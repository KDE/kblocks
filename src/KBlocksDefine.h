/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef KBLOCKSDEFINE_H
#define KBLOCKSDEFINE_H

#define PREPARE_AREA_WIDTH    5

#define FADE_ANIM_TIME_LINE   250
#define DROP_ANIM_TIME_LINE   250

typedef unsigned long         timeLong;

enum KBlocks_Game_Action {
    GameAction_None = 0,
    GameAction_Punish_Line,
    GameAction_Remove_Line,
    GameAction_Freeze_Piece_Color,
    GameAction_Freeze_Piece_X,
    GameAction_Freeze_Piece_Y,
    GameAction_New_Piece_X,
    GameAction_New_Piece_Y,
    GameAction_Max_Count
};

enum KBlocks_Game_Result {
    GameResult_None = 0,
    GameResult_Game_Over,
    GameResult_One_Step,
    GameResult_Next_Piece,
    GameResult_Remove_Line_1,
    GameResult_Remove_Line_2,
    GameResult_Remove_Line_3,
    GameResult_Remove_Line_4,
    GameResult_Max_Count
};

enum KBlocks_Game_State {
    GameState_Stop = 0,
    GameState_Running,
    GameState_Pause,
    GameState_Max_Count
};

enum KBlocks_Record_DataType {
    RecordDataType_GameCount = 0,
    RecordDataType_GameSeed,

    RecordDataType_Skipped,
    RecordDataType_MovePieceLeft,
    RecordDataType_MovePieceRight,
    RecordDataType_MovePieceUp,
    RecordDataType_MovePieceDown,
    RecordDataType_RotatePieceCW,
    RecordDataType_RotatePieceCCW,
    RecordDataType_GameOneStep,
    RecordDataType_PunishLineCount,
    RecordDataType_PunishLineSeed,

    RecordDataType_Max_Count
};

extern const char *KBlocksRecordText[RecordDataType_Max_Count];

#endif

