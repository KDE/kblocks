/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksGameRecorder.h"

const char * KBlocksRecordText[RecordDataType_Max_Count] =
{
    "GameCount",
    "PunishFactor",
    "StandbyMode",
    "FieldWidth",
    "FieldHeight",
    "ShowPieceCount",
    "MessagePoolSize",
    "MovePieceLeft",
    "MovePieceRight",
    "MovePieceDown",
    "RotatePieceCW",
    "RotatePieceCCW",
    "GameOneStep",
};

KBlocksGameRecorder::KBlocksGameRecorder(string fileName, bool isBinaryMode = true)
{
    mFileName = fileName;
    mBinaryMode = isBinaryMode;
    
    mGameRecord.clear();
}

KBlocksGameRecorder::~KBlocksGameRecorder()
{
    mGameRecord.clear();
}

void KBlocksGameRecorder::append(int index, int type, int value)
{
    _game_record_data tmpLastData = mGameRecord.back();
    if ((tmpLastData.index == index) && (tmpLastData.type == type))
    {
        switch(type)
        {
            case RecordDataType_GameCount:
            case RecordDataType_PunishFactor:
            case RecordDataType_StandbyMode:
            case RecordDataType_FieldWidth:
            case RecordDataType_FieldHeight:
            case RecordDataType_ShowPieceCount:
            case RecordDataType_MessagePoolSize:
                tmpLastData.value = value; // Overwrite with new value
                break;
            case RecordDataType_MovePieceLeft:
            case RecordDataType_MovePieceRight:
            case RecordDataType_MovePieceDown:
            case RecordDataType_RotatePieceCW:
            case RecordDataType_RotatePieceCCW:
            case RecordDataType_GameOneStep:
                tmpLastData.value += value; // Add up value with new value
                break;
            default:
                break;
        }
        mGameRecord.back() = tmpLastData;
    }
    else
    {
        tmpLastData.index = index;
        tmpLastData.type = type;
        tmpLastData.value = value;
        mGameRecord.push_back(tmpLastData);
    }
}

void KBlocksGameRecorder::save()
{
    FILE * pFile = fopen(mFileName, "w");
    if (mBinaryMode)
    {
        saveBinary(pFile);
    }
    else
    {
        saveText(pFile);
    }
    fclose(pFile);
}

void KBlocksGameRecorder::saveText(FILE * pFile)
{
    char tmpBuffer[256];
    list<_game_record_data>::iterator it;
    for(it = mylist.begin(); it != mylist.end(); it++)
    {
        sprintf(tmpBuffer, "%ld %s %d %d\n", it->time, KBlocksRecordText[it->type].c_str(), it->index, it->value);
        fputs(tmpBuffer, pFile);
    }
}

void KBlocksGameRecorder::saveBinary(FILE * pFile)
{
    list<_game_record_data>::iterator it;
    for(it = mylist.begin(); it != mylist.end(); it++)
    {
        writeByte(pFile, it->index);
        writeByte(pFile, it->type);
        writeInt(pFile, it->value);
    }
}

void KBlocksGameRecorder::writeByte(FILE * pFile, int value)
{
    int tmpByte = (value & 0xFF);
    fputc(tmpByte, pFile);
}

void KBlocksGameRecorder::writeInt(FILE * pFile, int value)
{
    int tmpByte = 0;
    for(int i = 0; i < 4; i++)
    {
        tmpByte = ((value >> (i * 8)) & 0xFF);
        fputc(tmpByte, pFile);
    }
}
