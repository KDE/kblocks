/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksGameReplayer.h"

KBlocksGameReplayer::KBlocksGameReplayer(const char * fileName, bool isBinaryMode)
{
    for(int i = 0; i < RecordDataType_Max_Count; ++i)
    {
        mRTMap[ KBlocksRecordText[i] ] = i;
    }
    mRTMap[string("MaxCount")] = -1;
    
    FILE * pFile = fopen(fileName, "r");
    
    if (!pFile)
    {
        mGameCount = 0;
        return;
    }
    
    if (isBinaryMode)
    {
        loadBinary(pFile);
    }
    else
    {
        loadText(pFile);
    }
    
    mGameCount = mReplayList.front().value;
    mReplayList.pop_front();
    mGameSeed = mReplayList.front().value;
    mSameSeed = (mReplayList.front().index == 1);
    mReplayList.pop_front();
    
    mStepLength = 1;
    
    fclose(pFile);
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
    if (stepLen > 1)
    {
        mStepLength = stepLen;
    }
    else
    {
        mStepLength = 1;
    }
}

bool KBlocksGameReplayer::getNextRecords(vector<KBlocksReplayData> * data)
{
    if (mReplayList.empty())
    {
        return false;
    }
    
    KBlocksReplayData tmpData;
    int tmpLength = mStepLength;
    while(tmpLength > 0)
    {
        tmpData = mReplayList.front();
        tmpLength -= tmpData.time;
        if (tmpLength > 0)
        {
            data->push_back(tmpData);
            mReplayList.pop_front();
        }
        else
        {
            mReplayList.front().time = -tmpLength;
        }
        if (mReplayList.empty())
        {
            return true;
        }
    }
    
    return true;
}

void KBlocksGameReplayer::loadText(FILE * pFile)
{
    int count = 0;
    char tmpString[256];
    KBlocksReplayData tmpData;
    mReplayList.clear();
    while(1)
    {
        count = fscanf(pFile, "%d %s %d %d", &(tmpData.time), tmpString, &(tmpData.index), &(tmpData.value));
        tmpData.type = mRTMap[string(tmpString)];
        if ((tmpData.type == -1) || (count != 4))
        {
            break;
        }
        mReplayList.push_back(tmpData);
    }
}

void KBlocksGameReplayer::loadBinary(FILE * pFile)
{
    KBlocksReplayData tmpData;
    mReplayList.clear();
    tmpData.time  = fgetc(pFile);
    tmpData.type  = fgetc(pFile);
    tmpData.index = fgetc(pFile);
    tmpData.value = fgetc(pFile);
    while(tmpData.time != EOF)
    {
        if (tmpData.type == RecordDataType_Skipped)
        {
            int tmpTime = tmpData.time;
            while(tmpData.type == RecordDataType_Skipped)
            {
                tmpData.time  = fgetc(pFile);
                tmpData.type  = fgetc(pFile);
                tmpData.index = fgetc(pFile);
                tmpData.value = fgetc(pFile);
                
                tmpTime += tmpData.time;
            }
            tmpData.time = tmpTime;
        }
        mReplayList.push_back(tmpData);
        tmpData.time  = fgetc(pFile);
        tmpData.type  = fgetc(pFile);
        tmpData.index = fgetc(pFile);
        tmpData.value = fgetc(pFile);
    }
}

