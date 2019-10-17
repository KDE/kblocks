/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksGameRecorder.h"
#include "utils.h"

KBlocksGameRecorder::KBlocksGameRecorder()
{
    mGameRecord.clear();
}

KBlocksGameRecorder::~KBlocksGameRecorder()
{
    mGameRecord.clear();
}

void KBlocksGameRecorder::append(int index, int type, int value)
{
    _game_record_data tmpLastData;
    tmpLastData.index = index;
    tmpLastData.type = type;
    tmpLastData.value = value;
    tmpLastData.time = Utils::getMillisecOfNow();
    mGameRecord.push_back(tmpLastData);
}

void KBlocksGameRecorder::save(const char *fileName, bool isBinaryMode)
{
    FILE *pFile = fopen(fileName, "w");
    if (isBinaryMode) {
        saveBinary(pFile);
    } else {
        saveText(pFile);
    }
    fclose(pFile);
}

void KBlocksGameRecorder::saveText(FILE *pFile)
{
    int tmpTime = 0;
    timeLong oldTime = mGameRecord.front().time;
    list<_game_record_data>::iterator it;
    for (it = mGameRecord.begin(); it != mGameRecord.end(); ++it) {
        tmpTime = (int)(it->time - oldTime);
        oldTime = it->time;
        fprintf(pFile, "%d %s %d %d\n", tmpTime, KBlocksRecordText[it->type], it->index, it->value);
    }
}

void KBlocksGameRecorder::saveBinary(FILE *pFile)
{
    int tmpTime = 0;
    timeLong oldTime = mGameRecord.front().time;
    list<_game_record_data>::iterator it;
    for (it = mGameRecord.begin(); it != mGameRecord.end(); ++it) {
        tmpTime = (int)(it->time - oldTime);
        oldTime = it->time;
        if (tmpTime > 255) {
            while (tmpTime > 255) {
                writeByte(pFile, 255);
                writeByte(pFile, RecordDataType_Skipped);
                writeByte(pFile, it->index);
                writeByte(pFile, it->value);
                tmpTime -= 255;
            }
        }
        writeByte(pFile, tmpTime);
        writeByte(pFile, it->type);
        writeByte(pFile, it->index);
        writeByte(pFile, it->value);
    }
}

void KBlocksGameRecorder::writeByte(FILE *pFile, int value)
{
    int tmpByte = (value & 0xFF);
    fputc(tmpByte, pFile);
}

