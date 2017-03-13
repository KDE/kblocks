/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSSCORE_H
#define KBLOCKSSCORE_H

enum KBlocks_Score_Type {
    KBlocksScore_Level_x_Factor = 0,
    KBlocksScore_Level_x_Level_x_Factor,
    KBlocksScore_Max_Count
};

class KBlocksScore
{
public:
    KBlocksScore();
    ~KBlocksScore();

    int getScorePoint();
    int getLineCount();
    int getGameLevel();

    void setLevelUpFactor(int type, int factor);
    void setScoreUpFactor(int factor);

    bool addScore(int lines);
    void clearScore();

private:
    int mPoint;
    int mLines;
    int mLevel;

    int mType;
    int mLFactor;
    int mSFactor;
};

#endif

