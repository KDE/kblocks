#include "KBlocksScore.h"

KBlocksScore::KBlocksScore()
{
    mPoint = 0;
    mLines = 0;
    mLevel = 0;
    
    mType = 0;
    mLFactor = 0;
    mSFactor = 0;
}

KBlocksScore::~KBlocksScore()
{
}

int KBlocksScore::getScorePoint()
{
    return mPoint;
}

int KBlocksScore::getLineCount()
{
    return mLines;
}

int KBlocksScore::getGameLevel()
{
    return mLevel;
}

void KBlocksScore::setLevelUpFactor(int type, int factor)
{
    mType = type;
    mLFactor = factor;
}

void KBlocksScore::setScoreUpFactor(int factor)
{
    mSFactor = factor;
}

void KBlocksScore::addScore(int lines)
{
    mLines += lines;
    
    switch(lines)
    {
        case 1:
            mPoint += mSFactor;
            break;
        case 2:
            mPoint += mSFactor * 3;
            break;
        case 3:
            mPoint += mSFactor * 6;
            break;
        case 4:
            mPoint += mSFactor * 10;
            break;
        default:
            break;
    }
    
    int levelUpScore = 0;
    switch(mType)
    {
        case KBlocksScore_Level_x_Factor:
            levelUpScore = mLevel * mLFactor;
            break;
        case KBlocksScore_Level_x_Level_x_Factor:
            levelUpScore = mLevel * mLevel * mLFactor;
            break;
        default:
            levelUpScore = mLevel * mLFactor;
            break;
    }
    
    if (mPoint >= levelUpScore)
    {
        mLevel++;
    }
}

void KBlocksScore::clearScore()
{
    mPoint = 0;
    mLines = 0;
    mLevel = 0;
}

