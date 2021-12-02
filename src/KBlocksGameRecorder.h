/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
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
