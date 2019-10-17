/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSGAMERECORDER_H
#define KBLOCKSGAMERECORDER_H

#include "KBlocksDefine.h"

#include <stdio.h>
#include <string>
#include <list>

using std::string;
using std::list;

struct _game_record_data {
    int index;
    int type;
    int value;
    timeLong time;
};

class KBlocksGameRecorder
{
public:
    KBlocksGameRecorder();
    ~KBlocksGameRecorder();

public:
    void append(int index, int type, int value);
    void save(const char *fileName, bool isBinaryMode = true);

private:
    void saveText(FILE *pFile);
    void saveBinary(FILE *pFile);
    void writeByte(FILE *pFile, int value);

private:
    list<_game_record_data> mGameRecord;
};

#endif
