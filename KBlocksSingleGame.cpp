#include "KBlocksSingleGame.h"
#include "KBlocksField.h"
#include "KBlocksPiece.h"

#define  ACTION_LIST_CAPACITY    64

KBlocksSingleGame::KBlocksSingleGame(int showPieceCount)
{
    mPieceCount = showPieceCount;
    
    mpField = new KBlocksField(10, 20);
    
    mpPieceList = new KBlocksPiece*[mPieceCount]();
    for(int i = 0; i < mPieceCount; i++)
    {
        mpPieceList[i] = new KBlocksPiece();
    }
    
    mpPieceGenerator = new KBlocksPieceGenerator();
    
    mLastGameResult = GameResult_None;
    
    mCurrentGameState = GameState_Stop;
    
    maActionType = new int[ACTION_LIST_CAPACITY]();
    maActionParam = new int[ACTION_LIST_CAPACITY]();
    
    mActionIndex = -1;
}

KBlocksSingleGame::~KBlocksSingleGame()
{
    delete [] maActionParam;
    delete [] maActionType;
    
    delete mpPieceGenerator;
    
    for(int i = 0; i < mPieceCount; i++)
    {
        delete mpPieceList[i];
    }
    delete [] mpPieceList;
    
    delete mpField;
}

KBlocksField* KBlocksSingleGame::getField()
{
    return mpField;
}

int KBlocksSingleGame::getPieceCount()
{
    return mPieceCount;
}

KBlocksPiece* KBlocksSingleGame::getPiece(int index)
{
    if ((index < 0) || (index > mPieceCount))
    {
        return 0;
    }
    return mpPieceList[index];
}

bool KBlocksSingleGame::setCurrentPiece(int xPos, int yPos, int rotation)
{
    if (mCurrentGameState != GameState_Start)
    {
        return false;
    }
    
    KBlocksPiece* tmpPiece = new KBlocksPiece();
    
    tmpPiece->fromValue(mpPieceList[0]->toValue());
    tmpPiece->setPosX(mpPieceList[0]->getPosX() + xPos);
    tmpPiece->setPosY(mpPieceList[0]->getPosY() + yPos);
    tmpPiece->setRotation(mpPieceList[0]->getRotation() + rotation);
    
    // TODO : Add a path check to prevent piece from setting to unreachable position
    
    if (checkPieceTouchGround(tmpPiece))
    {
        return false;
    }
    
    mpPieceList[0]->setPosX(tmpPiece->getPosX());
    mpPieceList[0]->setPosY(tmpPiece->getPosY());
    mpPieceList[0]->setRotation(tmpPiece->getRotation());
    
    return true;
}

bool KBlocksSingleGame::pushGameAction(int type, int param)
{
    if (mActionIndex == ACTION_LIST_CAPACITY)
    {
        return false;
    }
    
    mActionIndex++;
    maActionType[mActionIndex] = type;
    maActionParam[mActionIndex] = param;
    
    return true;
}

bool KBlocksSingleGame::popGameAction(int type, int * param)
{
    if (mActionIndex == -1)
    {
        return false;
    }
    
    for(int i = mActionIndex; i >= 0; i--)
    {
        if (type == maActionType[i])
        {
            *param = maActionParam[i];
            
            for(int j = i; j < mActionIndex; j++)
            {
                maActionType[j] = maActionType[j+1];
                maActionParam[j] = maActionParam[j+1];
            }
            
            mActionIndex--;
            
            return true;
        }
    }
    
    return false;
}

void KBlocksSingleGame::clearGameActions()
{
    mActionIndex = -1;
}

int KBlocksSingleGame::startGame(int seed)
{
    if (mCurrentGameState != GameState_Stop)
    {
        return mCurrentGameState;
    }
    
    mpPieceGenerator->genList(seed);
    
    for(int i = 0; i < mPieceCount; i++)
    {
        mpPieceList[i]->fromValue(mpPieceGenerator->getPiece());
    }
    
    mpPieceList[0]->setPosX(mpField->getWidth() / 2);
    mpPieceList[0]->setPosY(0);
    
    mpPieceList[1]->setPosX(2);
    mpPieceList[1]->setPosY(2);
    
    mCurrentGameState = GameState_Start;
    
    return mCurrentGameState;
}

int KBlocksSingleGame::pauseGame()
{
    if (mCurrentGameState == GameState_Start)
    {
        mCurrentGameState = GameState_Pause;
    }
    
    return mCurrentGameState;
}

int KBlocksSingleGame::resumeGame()
{
    if (mCurrentGameState == GameState_Pause)
    {
        mCurrentGameState = GameState_Start;
    }
    
    return mCurrentGameState;
}

int KBlocksSingleGame::stopGame()
{
    mCurrentGameState = GameState_Stop;
    return mCurrentGameState;
}

int KBlocksSingleGame::runStep()
{
    mLastGameResult = GameResult_None;
    
    if (mCurrentGameState != GameState_Start)
    {
        return mLastGameResult;
    }
    
    clearGameActions();
    
    if (!setCurrentPiece(0, 1, 0))
    {
        mLastGameResult = GameResult_Next_Piece;
        
        freezePieceToField(mpPieceList[0]);
        if (removeFieldLines())
        {
            mLastGameResult = GameResult_Remove_Line;
        }
        
        prepareNextPiece();
        if (checkPieceTouchGround(mpPieceList[0]))
        {
            mLastGameResult = GameResult_Game_Over;
            mCurrentGameState = GameState_Stop;
        }
    }
    
    return mLastGameResult;
}

int KBlocksSingleGame::runEvent(int gameEvent)
{
    mLastGameResult = GameResult_None;
    
    if (mCurrentGameState != GameState_Start)
    {
        return mLastGameResult;
    }
    
    if (GameEvent_None != gameEvent)
    {
        for(int i = 0; i < gameEvent; i++)
        {
            mpField->addPunishLine();
        }
    }
    
    return GameResult_None;
}

int KBlocksSingleGame::getLastGameResult()
{
    return mLastGameResult;
}

bool KBlocksSingleGame::checkPieceTouchGround(KBlocksPiece * p)
{
    for(int i = 0; i < 4; i++)
    {
        int posX = p->getCellPosX(i);
        int posY = p->getCellPosY(i);
        if (mpField->getCell(posX, posY))
        {
            return true;
        }
    }
    return false;
}

void KBlocksSingleGame::freezePieceToField(KBlocksPiece * p)
{
    for(int i = 0; i < 4; i++)
    {
        int posX = p->getCellPosX(i);
        int posY = p->getCellPosY(i);
        mpField->setCell(posX, posY, true);
    }
}

bool KBlocksSingleGame::removeFieldLines()
{
    bool isRemoved = false;
    int lineCount = 0;
    
    int maxLines = mpField->getHeight();
    for(int i = 0; i < maxLines; i++)
    {
        if (mpField->checkFilledLine(i))
        {
            pushGameAction(GameAction_Remove_Line, i);
            mpField->removeFilledLine(i);
            isRemoved = true;
            lineCount++;
        }
    }
    
    pushGameAction(GameAction_Remove_Count, lineCount);
    
    return isRemoved;
}

void KBlocksSingleGame::prepareNextPiece()
{
    for(int i = 0; i < mPieceCount - 1; i++)
    {
        mpPieceList[i]->fromValue(mpPieceList[i+1]->toValue());
    }
    
    mpPieceList[mPieceCount-1]->fromValue(mpPieceGenerator->getPiece());
    
    mpPieceList[0]->setPosX(mpField->getWidth() / 2);
    mpPieceList[0]->setPosY(0);
    
    mpPieceList[1]->setPosX(2);
    mpPieceList[1]->setPosY(2);
    
    for(int i = 0; i < 4; i++)
    {
        int posX = mpPieceList[0]->getCellPosX(i);
        int posY = mpPieceList[0]->getCellPosY(i);
        pushGameAction(GameAction_New_Piece_X, posX);
        pushGameAction(GameAction_New_Piece_Y, posY);
    }
}

