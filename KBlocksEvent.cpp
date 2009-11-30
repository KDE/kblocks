#include "KBlocksEvent.h"

KBlocksEvent::KBlocksEvent()
{
    mpGameLogic = 0;
}

KBlocksEvent::~KBlocksEvent()
{
}

void KBlocksEvent::setGameLogic(GameLogicInterface * p)
{
    mpGameLogic = p;
}

void KBlocksEvent::startGame()
{
    mpGameLogic->runEvent(GameEvent_Start_Game, -1);
}

void KBlocksEvent::pauseGame()
{
    mpGameLogic->runEvent(GameEvent_Pause_Game, -1);
}

void KBlocksEvent::resumeGame()
{
    mpGameLogic->runEvent(GameEvent_Resume_Game, -1);
}

void KBlocksEvent::stopGame()
{
    mpGameLogic->runEvent(GameEvent_Stop_Game, -1);
}

int KBlocksEvent::runGameAll(int * gameResult)
{
    int gameCount = mpGameLogic->getGameCount();
    
    gameResult = mpGameLogic->runStep(-1);
    
    return gameCount;
}

int KBlocksEvent::runGameByIndex(int gameIndex)
{
    int gameCount = mpGameLogic->getGameCount();
    int* gameResult = new int[gameCount]();
    
    gameResult = mpGameLogic->runStep(gameIndex);
    
    return gameResult[gameIndex];
}
