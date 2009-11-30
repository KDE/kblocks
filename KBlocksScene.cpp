#include "KBlocksScene.h"

#include "settings.h"

#include <KGameDifficulty>

KBlocksScene::KBlocksScene(int capacity)
{
    mpGameEvent = 0;
    
    mMaxCapacity = capacity;
    mGroupCount = 0;
    maGroupList = 0;
    maControlList = 0;
    
    QString themeFile(Settings::theme());
    mpGrafx = new KBlocksGraphics(themeFile);
    mpSnd = new KBlocksSound(themeFile);

    setSceneRect(0, 0, mpGrafx->m_View_Size_Width, mpGrafx->m_View_Size_Height);
    
    setItemIndexMethod(NoIndex);
    
    mCurrentGameState = GameState_Stop;
}

KBlocksScene::~KBlocksScene()
{
    delete mpGrafx;
    delete mpSnd;
}

void KBlocksScene::setEventHandler(KBlocksEvent * p)
{
    mpGameEvent = p;
}

void KBlocksScene::drawBackground( QPainter * painter, const QRectF & rect )
{
    if (mpGrafx->renderer()->isValid())
    {
        mpGrafx->renderer()->render(painter, QString("BACKGROUND"), rect);
    }
}

void KBlocksScene::createGameItemGroups(int groupCount, GameLogicInterface * p)
{
    if (groupCount > mMaxCapacity)
    {
        mGroupCount = mMaxCapacity;
    }
    else
    {
        mGroupCount = groupCount;
    }
    
    maGroupList = new KBlocksItemGroup*[mGroupCount]();
    maControlList = new KBlocksControl*[mGroupCount]();
    
    maGameScoreList = new KBlocksScore*[mGroupCount]();
    
    for(int i = 0; i < mGroupCount; i++)
    {
        maGroupList[i] = new KBlocksItemGroup(p->getSingleGame(i), mpGrafx);
        maControlList[i] = new KBlocksControl();
        maGameScoreList[i] = new KBlocksScore();
        
        maGroupList[i]->setControlHandler(maControlList[i]);
        maGroupList[i]->setSoundHandler(mpSnd);
        maGroupList[i]->setGroupID(i);
        connect(maGroupList[i], SIGNAL(stepEmited(int)), this, SLOT(runGameOneStep(int)));
        
        maControlList[i]->setSingleGame(p->getSingleGame(i));
        
        maGameScoreList[i]->setLevelUpFactor(KBlocksScore_Level_x_Level_x_Factor, 10000);
        maGameScoreList[i]->setScoreUpFactor(10);
        
        addItem(maGroupList[i]);
    }
    
    updateDimensions();
	
    //Our Message Item, hidden by default
    mMessageBox = new KGamePopupItem();
    mMessageBox->setMessageOpacity(0.9);
    addItem(mMessageBox);
}

void KBlocksScene::deleteGameItemGroups()
{
    removeItem(mMessageBox);
    delete mMessageBox;

    for(int i = 0; i < mGroupCount; i++)
    {
        delete maControlList[i];
        removeItem(maGroupList[i]);
        delete maGroupList[i];
    }
    delete [] maControlList;
    delete [] maGroupList;
}

KBlocksGraphics* KBlocksScene::getGraphicHandler()
{
    return mpGrafx;
}

KBlocksSound* KBlocksScene::getSoundHandler()
{
    return mpSnd;
}

KBlocksItemGroup* KBlocksScene::getItemGroup(int index)
{
    return maGroupList[index];
}

KBlocksControl* KBlocksScene::getControlHandler(int index)
{
    return maControlList[index];
}

void KBlocksScene::readSettings(const QSize & viewSize)
{
    if (mpGrafx->theme()->fileName()!=Settings::theme())
    {
        mpGrafx->loadTheme(Settings::theme());
        mpSnd->loadTheme(Settings::theme());
        mpGrafx->adjustForSize(viewSize);
        updateDimensions();
    }
}

void KBlocksScene::viewScaled(const QSize& /*newsize*/)
{
    //Temporarily halt game timer while resizing elements
    //if (gameState==Game_Active) stepTimer.stop();
    //grafx->adjustForSize(newsize);
    //updateDimensions();
    //Do not restart if game was paused
    //if (gameState==Game_Active) stepTimer.start();
}

void KBlocksScene::updateDimensions()
{
    // TODO : Reset item position and scale
    int width = (mGroupCount >= SCENE_GAMES_PER_WIDTH) ? SCENE_GAMES_PER_WIDTH : (mGroupCount % SCENE_GAMES_PER_WIDTH);
    int height = (int)(mGroupCount / (SCENE_GAMES_PER_WIDTH + 1)) + 1;
    
    setSceneRect(0, 0, mpGrafx->m_View_Size_Width * width,
                       mpGrafx->m_View_Size_Height * height);
    
    for(int i = 0; i < mGroupCount; i++)
    {
        int left = mpGrafx->m_View_Size_Width * (i % SCENE_GAMES_PER_WIDTH);
        int top = mpGrafx->m_View_Size_Height * ((int)(i / SCENE_GAMES_PER_WIDTH));
        
        maGroupList[i]->setPos(left, top);
        maGroupList[i]->refreshPosition(mpGrafx);
    }
}

void KBlocksScene::setSoundsEnabled(bool enabled)
{
    mpSnd->setSoundsEnabled(enabled);
}

void KBlocksScene::startGame()
{
    if (mCurrentGameState != GameState_Stop)
    {
        return;
    }
    
    mpGameEvent->startGame();
    
    for(int i = 0; i < mGroupCount; i++)
    {
        maGroupList[i]->startGame();
    }
    
    QTimer::singleShot(500, this, SLOT(greetPlayer()));
    mCurrentGameState = GameState_Start;
    
    KGameDifficulty::standardLevel level = KGameDifficulty::level();
    Settings::setDifficulty((int) level);
    int levelUpTime = 0;
    if (KGameDifficulty::level() == KGameDifficulty::Medium)
    {
        levelUpTime = 5;
    }
    else if (KGameDifficulty::level() == KGameDifficulty::Hard)
    {
        levelUpTime = 10;
    }
    for(int i = 0; i < mGroupCount; i++)
    {
        for(int j = 0; j < levelUpTime; j++)
        {
            maGroupList[i]->gameLevelUp();
        }
    }
    KGameDifficulty::setRunning(true);
}

void KBlocksScene::pauseGame(bool flag, bool fromUser)
{
    if (mCurrentGameState == GameState_Start)
    {
        if ((!fromUser) || flag)
        {
            mCurrentGameState = GameState_Pause;
            mpGameEvent->pauseGame();
            for(int i = 0; i < mGroupCount; i++)
            {
                maGroupList[i]->pauseGame();
            }
            KGameDifficulty::setRunning(false);
        }
    }
    else if (mCurrentGameState == GameState_Pause)
    {
        if (!(fromUser && flag))
        {
            mCurrentGameState = GameState_Start;
            mpGameEvent->resumeGame();
            for(int i = 0; i < mGroupCount; i++)
            {
                maGroupList[i]->resumeGame();
            }
            KGameDifficulty::setRunning(true);
        }
    }
}

void KBlocksScene::stopGame()
{
    mCurrentGameState = GameState_Stop;
    
    mpGameEvent->stopGame();
    
    for(int i = 0; i < mGroupCount; i++)
    {
        maGroupList[i]->stopGame();
    }
    
    KGameDifficulty::setRunning(false);
}

int KBlocksScene::getGameState()
{
    return mCurrentGameState;
}

void KBlocksScene::runGameOneStep(int gameIndex)
{
    // One of the game is stepped, then run that game for one step
    int gameResult = mpGameEvent->runGameByIndex(gameIndex);
    
    if ((gameResult == GameResult_Next_Piece) || (gameResult == GameResult_Remove_Line))
    {
        mpSnd->playSound("Sound_Block_Fall");
        
        maGroupList[gameIndex]->prepareNextPiece();
        
        if (gameResult == GameResult_Remove_Line)
        {
            mpSnd->playSound("Sound_Block_Remove");
            
            int lineCount = maControlList[gameIndex]->getRemovedCount();
            int oldLevel = maGameScoreList[gameIndex]->getGameLevel();
            
            maGameScoreList[gameIndex]->addScore(lineCount);
            
            if (oldLevel < maGameScoreList[gameIndex]->getGameLevel())
            {
                maGroupList[gameIndex]->gameLevelUp();
            }
            
            // TODO : maybe add score display for all other games.
            if (gameIndex == 0)
            {
                emit scoreChanged(maGameScoreList[gameIndex]->getScorePoint(),
                                  maGameScoreList[gameIndex]->getLineCount(), 
                                  maGameScoreList[gameIndex]->getGameLevel());
            }
        }
    }
    else
    {
        maGroupList[gameIndex]->updateGroupItems();
    }
    
    if (gameResult == GameResult_Game_Over)
    {
        // TODO : one game is over...
        maGroupList[gameIndex]->stopGame();
        if (gameIndex == 0)
        {
            emit isHighscore(maGameScoreList[gameIndex]->getScorePoint(), maGameScoreList[gameIndex]->getGameLevel());
            QTimer::singleShot(500, this, SLOT(gameOverPlayer()));
        }
    }
}

void KBlocksScene::greetPlayer()
{
    QString greets(i18n("Game started")); 
    mMessageBox->setMessageTimeout( 2000 );
    mMessageBox->showMessage( greets, KGamePopupItem::TopLeft );
}

void KBlocksScene::gameOverPlayer()
{
    QString gameOver(i18n("Game Over")); 
    mMessageBox->setMessageTimeout( 2000 );
    mMessageBox->showMessage( gameOver, KGamePopupItem::TopLeft );
}


