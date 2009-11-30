#ifndef KBLOCKSSCORE_H
#define KBLOCKSSCORE_H

enum KBlocks_Score_Type {
    KBlocksScore_Level_x_Factor = 0,
    KBlocksScore_Level_x_Level_x_Factor,
    KBlocksScore_Max_Count,
};

class KBlocksScore {
    public:
        KBlocksScore();
        ~KBlocksScore();
        
        int getScorePoint();
        int getLineCount();
        int getGameLevel();
        
        void setLevelUpFactor(int type, int factor);
        void setScoreUpFactor(int factor);
        
        void addScore(int lines);
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
