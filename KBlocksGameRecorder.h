/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSGAMERECORDER_H
#define KBLOCKSGAMERECORDER_H

#include <stdio.h>
#include <string>
#include <list>

enum KBlocksRecord_DataType
{
    RecordDataType_GameCount = 0,
    RecordDataType_PunishFactor,
    RecordDataType_StandbyMode,
    
    RecordDataType_FieldWidth,
    RecordDataType_FieldHeight,
    RecordDataType_ShowPieceCount,
    RecordDataType_MessagePoolSize,
    
    RecordDataType_MovePieceLeft,
    RecordDataType_MovePieceRight,
    RecordDataType_MovePieceDown,
    RecordDataType_RotatePieceCW,
    RecordDataType_RotatePieceCCW,
    RecordDataType_GameOneStep,
    
    RecordDataType_Max_Count,
};

struct _game_record_data
{
    int index;
    int type;
    int value;
    long time;
};

class KBlocksGameRecorder
{
    public:
        KBlocksGameRecorder(string fileName, bool isBinaryMode = true);
        ~KBlocksGameRecorder();
        
    public:
        void append(int index, int type, int value, int time);
        void save();
        
    private:
        void saveText(FILE * pFile);
        void saveBinary(FILE * pFile);
        void writeByte(FILE * pFile, int value);
        void writeInt(FILE * pFile, int value);
        
        bool compareData(_game_record_data first, _game_record_data second);
        
    private:
        string mFileName;
        bool mBinaryMode;
        
        std::list<_game_record_data> mGameRecord;
};

#endif