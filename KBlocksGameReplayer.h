/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSGAMEREPLAYER_H
#define KBLOCKSGAMEREPLAYER_H

#include "KBlocksDefine.h"

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>

using std::string;
using std::vector;
using std::list;
using std::map;

struct KBlocksReplayData {
    int index;
    int type;
    int value;
    int time;
};

class KBlocksGameReplayer
{
public:
    explicit KBlocksGameReplayer(const char *fileName, bool isBinaryMode = true);
    ~KBlocksGameReplayer();

public:
    void setStepLength(int stepLen);

    int getGameCount();
    int getGameSeed();
    bool isSameSeed();

    bool getNextRecords(vector<KBlocksReplayData> *data);

private:
    void loadText(std::ifstream &pFile);
    void loadBinary(std::ifstream &pFile);

private:
    int mGameCount;
    int mGameSeed;
    bool mSameSeed;
    int mStepLength;
    list<KBlocksReplayData> mReplayList;
    map<string, int> mRTMap;
};

#endif
