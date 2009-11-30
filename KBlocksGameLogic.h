#ifndef KBLOCKSGAMELOGIC_H
#define KBLOCKSGAMELOGIC_H

#include "GameLogicInterface.h"
#include "SingleGameInterface.h"

#include "KBlocksSingleGame.h"

#include "KBlocksDefine.h"

class KBlocksGameLogic : public GameLogicInterface {
    public:
        KBlocksGameLogic(int capacity);
        ~KBlocksGameLogic();
        
    public:
        void createSingleGames(int gameCount);
        void deleteSingleGames();
        
        void setGameSeed(int seed);
        
        int getGameMaxCapacity();
        int getGameCount();
        KBlocksSingleGame* getSingleGame(int index);
        
        int* runStep(int gameIndex = -1);
        int runEvent(int gameEvent, int gameIndex = -1);
        
    protected:
        KBlocksSingleGame** maGameList;
        
    private:
        int mGameMax;
        int mGameCount;
		int mGameSeed;
};

#endif
