/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Mauricio Piacentini <mauricio@tabuleiro.com>       *
*                      Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksWin.h"

#include <limits.h>
#include <stdlib.h>

#include <KConfigDialog>
#include <kstandardgameaction.h>
#include <KStandardAction>
#include <KIcon>
#include <kscoredialog.h>
#include <KLocale>
#include <KToggleAction>
#include <KActionCollection>
#include <KStatusBar>
#include <KgDifficulty>

#include <QPixmapCache>

#define USE_UNSTABLE_LIBKDEGAMESPRIVATE_API
#include <libkdegamesprivate/kgamethemeselector.h>

#include "settings.h"

KBlocksWin::KBlocksWin(KBlocksGameLogic * p, KBlocksPlayManager * pM, int capacity, int gamecount) : KXmlGuiWindow()
{
    //Use up to 3MB for global application pixmap cache
    QPixmapCache::setCacheLimit(3*1024);
    
    mFirstInit = true;
    
    mpKBPlayer = new KBlocksKeyboardPlayer(this);
    mpAIPlayer = new KBlocksAIPlayer();
    
    mMaxGameCapacity = capacity;
    mGameCount = gamecount;
    mpGameLogic = p;
    mpPlayManager = pM;
    
    mpGameScene = new KBlocksScene(mpGameLogic, capacity);
    
    mpGameView = new KBlocksView(mpGameScene, this);
	mpGameView->show();
    setCentralWidget(mpGameView);
    connect(mpGameView, SIGNAL(focusEvent(bool)), this, SLOT(focusEvent(bool)));
    
    setAutoSaveSettings();
    
    setupGUILayout();
}

KBlocksWin::~KBlocksWin()
{
    delete mpGameView;
    delete mpGameScene;
    delete mpAIPlayer;
    delete mpKBPlayer;
}

void KBlocksWin::setGamesPerLine(int count)
{
    mpGameScene->setGamesPerLine(count);
}

void KBlocksWin::setGameAnimEnabled(bool flag)
{
    mpGameScene->setGameAnimEnabled(flag);
    mGameAnim = flag;
}

void KBlocksWin::setWaitForAllUpdate(bool flag)
{
    mpGameScene->setWaitForAllUpdate(flag);
    mWaitForAll = flag;
}

void KBlocksWin::setUpdateInterval(int interval)
{
    mpGameScene->setUpdateInterval(interval);
}

void KBlocksWin::addScore(int gameIndex, int lineCount)
{
    mpGameScene->addScore(gameIndex, lineCount);
}

void KBlocksWin::startGame()
{
    if (mFirstInit)
    {
        mFirstInit = false;
        return;
    }
    
    srand(time(0));
    mpGameLogic->setGameSeed(rand());
    if (mpGameLogic->startGame(mGameCount))
    {
        mpPlayManager->startGame();
        
        mpGameScene->createGameItemGroups(mGameCount, false);
        mpGameScene->startGame();
        
        int levelUpTime = 0;
        switch ((int) Kg::difficultyLevel())
        {
            case KgDifficultyLevel::Medium:
                levelUpTime = 5;
                break;
            case KgDifficultyLevel::Hard:
                levelUpTime = 10;
                break;
        }
        mpGameLogic->levelUpGame(levelUpTime);
        
        Kg::difficulty()->setGameRunning(true);
    }
    else
    {
        stopGame();
        startGame();
    }

    statusBar()->changeItem( i18n("Points: %1 - Lines: %2 - Level: %3", 0, 0, 0), 0 );
}

void KBlocksWin::stopGame()
{
    if (mpGameLogic->stopGame())
    {
        mpPlayManager->stopGame();
        
        mpGameScene->stopGame();
        mpGameScene->deleteGameItemGroups();
        
        Kg::difficulty()->setGameRunning(false);
    }
}

void KBlocksWin::pauseGame()
{
    mpGameLogic->pauseGame(m_pauseAction->isChecked());
    mpPlayManager->pauseGame(m_pauseAction->isChecked());
    mpGameScene->pauseGame(m_pauseAction->isChecked());
    
    Kg::difficulty()->setGameRunning(!m_pauseAction->isChecked());
}

void KBlocksWin::singleGame()
{
    mpPlayManager->stopGame();
    mpPlayManager->clearGamePlayer();
    mpPlayManager->addGamePlayer(mpKBPlayer, -1, -1);
    mGameCount = 1;
    
    startGame();
}

void KBlocksWin::pveStepGame()
{
    mpPlayManager->stopGame();
    mpPlayManager->clearGamePlayer();
    mpPlayManager->addGamePlayer(mpKBPlayer, -1, -1);
    mpPlayManager->addGamePlayer(mpAIPlayer, 200, 50);
    mGameCount = 2;
    
    mpGameLogic->setGameStandbyMode(true);
    setWaitForAllUpdate(true);
    
    startGame();
}

void KBlocksWin::focusEvent(bool flag)
{
    if (!flag)
    {
        if (m_pauseAction->isChecked())
        {
            return;
        }
    }
    mpGameLogic->pauseGame(flag);
    mpPlayManager->pauseGame(flag);
    mpGameScene->pauseGame(flag, true);
}

bool KBlocksWin::queryExit()
{
    Settings::self()->writeConfig();
    return true;
}

void KBlocksWin::showHighscore()
{
    KScoreDialog ksdialog(KScoreDialog::Name | KScoreDialog::Level | KScoreDialog::Score, this);
    ksdialog.initFromDifficulty(Kg::difficulty());
    ksdialog.exec();
}

void KBlocksWin::configureSettings()
{
    if ( KConfigDialog::showDialog("settings") )
    {
        return;
    }
    KConfigDialog *dialog = new KConfigDialog(this, "settings", Settings::self());
    dialog->addPage(new KGameThemeSelector(dialog, Settings::self()), i18n("Theme"), "games-config-theme");
	dialog->setFaceType(KConfigDialog::Plain); //only one page -> no page selection necessary
    connect(dialog, SIGNAL(settingsChanged(QString)), mpGameView, SLOT(settingsChanged()));
    //connect(dialog, SIGNAL(hidden()), view, SLOT(resumeFromConfigure()));
    dialog->show();
}

void KBlocksWin::onScoreChanged(int index, int points, int lines, int level)
{
    if (index == 0) // TODO : game id?? multi game display??
    {
        statusBar()->changeItem( i18n("Points: %1 - Lines: %2 - Level: %3", points, lines, level), 0 );
    }
}

void KBlocksWin::onIsHighscore(int index, int points, int level)
{
    if (index == 0) // TODO : game id?? multi game display??
    {
        KScoreDialog ksdialog( KScoreDialog::Name | KScoreDialog::Level | KScoreDialog::Score, this );
        ksdialog.initFromDifficulty(Kg::difficulty());
        KScoreDialog::FieldInfo info;
        info[KScoreDialog::Score].setNum( points );
        info[KScoreDialog::Level].setNum( level );
        if ( ksdialog.addScore( info ) )
        {
            ksdialog.exec();
        }
    }
}

void KBlocksWin::levelChanged()
{
    //Scene reads the difficulty level for us
    startGame();
    mpGameView->setFocus(Qt::MouseFocusReason);
}

void KBlocksWin::setSoundsEnabled(bool enabled)
{
    mpGameScene->setSoundsEnabled(enabled);
    Settings::setSounds(enabled);
}

void KBlocksWin::setupGUILayout()
{
    QAction *action;
    
    action = KStandardGameAction::gameNew(this, SLOT(singleGame()), actionCollection());
    action->setText(i18n("Single Game"));
    actionCollection()->addAction( QLatin1String( "newGame" ), action);
    
    action = new KAction(this);
    action->setText(i18n("Human vs AI"));
    actionCollection()->addAction( QLatin1String( "pve_step" ), action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(pveStepGame()));
    
    m_pauseAction = KStandardGameAction::pause(this, SLOT(pauseGame()), actionCollection());
    actionCollection()->addAction( QLatin1String( "pauseGame" ), m_pauseAction);
    
    action = KStandardGameAction::highscores(this, SLOT(showHighscore()), actionCollection());
    actionCollection()->addAction( QLatin1String( "showHighscores" ), action);
    
    action = KStandardGameAction::quit(this, SLOT(close()), actionCollection());
    actionCollection()->addAction( QLatin1String( "quit" ), action);
    
    KStandardAction::preferences(this, SLOT(configureSettings()), actionCollection());
    
    KAction* soundAction = new KToggleAction(i18n("&Play sounds"), this);
    soundAction->setChecked(Settings::sounds());
    actionCollection()->addAction( QLatin1String( "sounds" ), soundAction);
    connect(soundAction, SIGNAL(triggered(bool)), this, SLOT(setSoundsEnabled(bool)));
    
    // TODO
    statusBar()->insertItem( i18n("Points: 0 - Lines: 0 - Level: 0"), 0 );
    connect(mpGameScene, SIGNAL(scoreChanged(int,int,int,int)), this,  SLOT(onScoreChanged(int,int,int,int)));
    connect(mpGameScene, SIGNAL(isHighscore(int,int,int)), this,  SLOT(onIsHighscore(int,int,int)));
    
    Kg::difficulty()->addStandardLevelRange(
        KgDifficultyLevel::Easy, KgDifficultyLevel::Hard
    );
    KgDifficultyGUI::init(this);
    connect(Kg::difficulty(), SIGNAL(currentLevelChanged(const KgDifficultyLevel*)), SLOT(levelChanged()));
    
    setupGUI();
}

