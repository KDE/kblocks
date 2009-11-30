#ifndef KBLOCKSEVENT_H
#define KBLOCKSEVENT_H

#include "GameLogicInterface.h"

#include "KBlocksDefine.h"

class KBlocksEvent
{
    public:
        KBlocksEvent();
        ~KBlocksEvent();
        
        void setGameLogic(GameLogicInterface * p);
        
        void startGame();
        void pauseGame();
        void resumeGame();
        void stopGame();
        
        int runGameAll(int * gameResult);
        int runGameByIndex(int gameIndex);

    protected:
        GameLogicInterface* mpGameLogic;
};

#endif
