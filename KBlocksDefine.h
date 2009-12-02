/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef KBLOCKSDEFINE_H
#define KBLOCKSDEFINE_H

enum KBlocks_Game_Event {
    GameEvent_None = 0,
    
    GameEvent_Punish_Line_1,
    GameEvent_Punish_Line_2,
    GameEvent_Punish_Line_3,
    GameEvent_Punish_Line_4,
    
    GameEvent_Start_Game,
    GameEvent_Pause_Game,
    GameEvent_Resume_Game,
    GameEvent_Stop_Game,
    
    GameEvent_Max_Count,
};

enum KBlocks_Game_Action {
    GameAction_None = 0,
    GameAction_Remove_Line,
    GameAction_Remove_Count,
    GameAction_New_Piece_X,
    GameAction_New_Piece_Y,
    GameAction_Max_Count,
};

enum KBlocks_Game_Result {
    GameResult_None = 0,
    GameResult_Next_Piece,
    GameResult_Remove_Line,
    GameResult_Game_Over,
    GameResult_Max_Count,
};

enum KBlocks_Game_State {
    GameState_Stop = 0,
    GameState_Start,
    GameState_Pause,
    GameState_Max_Count,
};

#endif
