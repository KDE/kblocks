/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksGameReplayer.h"

#include <sstream>

#include "kblocks_replay_debug.h"

KBlocksGameReplayer::KBlocksGameReplayer(const char *fileName, bool isBinaryMode)
{
    // Map data types to strings for reading text file
    for (int i = 0; i < RecordDataType_Max_Count; ++i) {
        mRTMap[ KBlocksRecordText[i] ] = i;
    }
    mRTMap[string("MaxCount")] = -1;

    // Set default variables in case loading the file fails
    mGameCount = 0;
    mGameSeed = 0;
    mSameSeed = false;
    mStepLength = 1;

    // Open replay file
    std::ifstream replayFile;
    if (isBinaryMode) {
        replayFile.open(fileName, std::ios::binary);
    } else {
        replayFile.open(fileName);
    }

    // Check that replay file was opened successfully
    if (!replayFile.is_open()) {
        qCWarning(KBReplay) << "Unable to open file " << fileName;
        return;
    }

    if (isBinaryMode) {
        loadBinary(replayFile);
    } else {
        loadText(replayFile);
    }

    // Check that more than two Replay steps have been loaded
    // The two first steps set the required variables.
    if (mReplayList.size() < 2) {
        qCWarning(KBReplay) << "Problem loading replay file" << fileName;
        return;
    }

    mGameCount = mReplayList.front().value;
    mReplayList.pop_front();
    mGameSeed = mReplayList.front().value;
    mSameSeed = (mReplayList.front().index == 1);
    mReplayList.pop_front();

    replayFile.close();
}

KBlocksGameReplayer::~KBlocksGameReplayer()
{
}

int KBlocksGameReplayer::getGameCount()
{
    return mGameCount;
}

int KBlocksGameReplayer::getGameSeed()
{
    return mGameSeed;
}

bool KBlocksGameReplayer::isSameSeed()
{
    return mSameSeed;
}

void KBlocksGameReplayer::setStepLength(int stepLen)
{
    if (stepLen > 1) {
        mStepLength = stepLen;
    } else {
        mStepLength = 1;
    }
}

bool KBlocksGameReplayer::getNextRecords(vector<KBlocksReplayData> *data)
{
    if (mReplayList.empty()) {
        return false;
    }

    KBlocksReplayData tmpData;
    int tmpLength = mStepLength;
    while (tmpLength > 0) {
        tmpData = mReplayList.front();
        tmpLength -= tmpData.time;
        if (tmpLength > 0) {
            data->push_back(tmpData);
            mReplayList.pop_front();
        } else {
            mReplayList.front().time = -tmpLength;
        }
        if (mReplayList.empty()) {
            return true;
        }
    }

    return true;
}

void KBlocksGameReplayer::loadText(std::ifstream &replayFile)
{
    std::string line;
    std::istringstream inStream;
    std::string tmpString;
    KBlocksReplayData tmpData;
    mReplayList.clear();
    do {
        std::getline(replayFile, line);
        inStream.str(line);
        inStream >> tmpData.time >> tmpString >> tmpData.index >> tmpData.value;
        tmpData.type = mRTMap[tmpString];
        if ((tmpData.type == -1) || inStream.fail()) {
            break;
        }
        mReplayList.push_back(tmpData);
        inStream.clear();
    } while (!replayFile.eof());
}

void KBlocksGameReplayer::loadBinary(std::ifstream &replayFile)
{
    KBlocksReplayData tmpData;
    mReplayList.clear();

    tmpData.time  = replayFile.get();
    tmpData.type  = replayFile.get();
    tmpData.index = replayFile.get();
    tmpData.value = replayFile.get();

    do {
        if (tmpData.type == RecordDataType_Skipped) {
            int tmpTime = tmpData.time;
            while (tmpData.type == RecordDataType_Skipped) {
                tmpData.time  = replayFile.get();
                tmpData.type  = replayFile.get();
                tmpData.index = replayFile.get();
                tmpData.value = replayFile.get();

                tmpTime += tmpData.time;
            }
            tmpData.time = tmpTime;
        }
        mReplayList.push_back(tmpData);
        tmpData.time  = replayFile.get();
        tmpData.type  = replayFile.get();
        tmpData.index = replayFile.get();
        tmpData.value = replayFile.get();
    } while (!replayFile.eof());
}

