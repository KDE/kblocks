/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2009-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/
#include "KBlocksWin.h"

#include <limits.h>

#include <KConfigDialog>
#include <kstandardgameaction.h>
#include <KStandardAction>
#include <highscore/kscoredialog.h>
#include <KLocalizedString>
#include <KToggleAction>
#include <KActionCollection>
#include <QStatusBar>
#include <KgDifficulty>

#include <QPixmapCache>
#include <QPointer>
#include <QLabel>
#include <QRandomGenerator>

#define USE_UNSTABLE_LIBKDEGAMESPRIVATE_API
#include <libkdegamesprivate/kgamethemeselector.h>

#include "GameLogicInterface.h"
#include "KBlocksScene.h"
#include "SceneInterface.h"
#include "settings.h"

KBlocksWin::KBlocksWin(
    GameLogicInterface *p,
    GraphicsInterface *graphics,
    SoundInterface *sound,
    KBlocksPlayManager *pM,
    int capacity,
    int gamecount
) : KXmlGuiWindow()
{
    //Use up to 3MB for global application pixmap cache
    QPixmapCache::setCacheLimit(3 * 1024);

    mpKBPlayer = new KBlocksKeyboardPlayer(this);
    mpAIPlayer = new KBlocksAIPlayer();

    mMaxGameCapacity = capacity;
    mGameCount = gamecount;
    mpGameLogic = p;
    connect(mpGameLogic, &GameLogicInterface::allGamesStopped,
            this, &KBlocksWin::onAllGamesStopped);

    mpPlayManager = pM;

    mpGameScene = new KBlocksScene(mpGameLogic, graphics, sound, capacity);

    connect(mpKBPlayer, &KBlocksKeyboardPlayer::blockMoved,
            mpGameScene, &SceneInterface::playMoveSound);
    connect(mpKBPlayer, &KBlocksKeyboardPlayer::blockDropped,
            mpGameScene, &SceneInterface::playDropSound);

    mpGameView = new KBlocksView(mpGameScene, this);
    mpGameView->show();
    setCentralWidget(mpGameView);
    connect(mpGameView, &KBlocksView::focusEvent, this, &KBlocksWin::focusEvent);

    setAutoSaveSettings();
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
    mpGameLogic->setGameSeed(QRandomGenerator::global()->generate());
    if (mpGameLogic->startGame(mGameCount)) {
        mpPlayManager->startGame();

        mpGameScene->createGameItemGroups(mGameCount, false);
        mpGameScene->startGame();

        int levelUpTime = 0;
        switch ((int) Kg::difficultyLevel()) {
        case KgDifficultyLevel::Medium:
            levelUpTime = 5;
            break;
        case KgDifficultyLevel::Hard:
            levelUpTime = 10;
            break;
        }
        mpGameLogic->levelUpGame(levelUpTime);

        Kg::difficulty()->setGameRunning(true);
    } else {
        stopGame();
        startGame();
    }

    mpGameView->fitInView(mpGameScene->sceneRect(), Qt::KeepAspectRatio);

    mScore->setText(i18n("Points: %1 - Lines: %2 - Level: %3", 0, 0, 0));

    m_pauseAction->setEnabled(true);
    m_pauseAction->setChecked(false);
}

void KBlocksWin::stopGame()
{
    if (mpGameLogic->hasSingleGames()) {
        mpGameScene->stopGame();
        mpGameLogic->deleteSingleGames();
        // Clear the game field
        mpGameScene->deleteGameItemGroups();
    }
}

void KBlocksWin::pauseGame()
{
    mpGameLogic->pauseGame(m_pauseAction->isChecked());
    mpPlayManager->pauseGame(m_pauseAction->isChecked());
    mpGameScene->pauseGame(m_pauseAction->isChecked());

    mpKBPlayer->pauseGame(m_pauseAction->isChecked());
    mpAIPlayer->pauseGame(m_pauseAction->isChecked());

    Kg::difficulty()->setGameRunning(!m_pauseAction->isChecked());
}

void KBlocksWin::onAllGamesStopped()
{
    mpPlayManager->stopGame();
    m_pauseAction->setEnabled(false);
    Kg::difficulty()->setGameRunning(false);
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
    if (!flag) {
        if (m_pauseAction->isChecked()) {
            return;
        }
    }
    mpGameLogic->pauseGame(flag);
    mpPlayManager->pauseGame(flag);
    mpGameScene->pauseGame(flag, true);
}

void KBlocksWin::closeEvent(QCloseEvent *event)
{
    Settings::self()->save();
    KXmlGuiWindow::closeEvent(event);
}

void KBlocksWin::showHighscore()
{
    KScoreDialog ksdialog(KScoreDialog::Name | KScoreDialog::Level | KScoreDialog::Score, this);
    ksdialog.initFromDifficulty(Kg::difficulty());
    ksdialog.exec();
}

void KBlocksWin::configureSettings()
{
    if (KConfigDialog::showDialog(QStringLiteral("settings"))) {
        return;
    }
    KConfigDialog *dialog = new KConfigDialog(this, QStringLiteral("settings"), Settings::self());
    dialog->addPage(new KGameThemeSelector(dialog, Settings::self()), i18n("Theme"), QStringLiteral("games-config-theme"));
    dialog->setFaceType(KConfigDialog::Plain); //only one page -> no page selection necessary
    connect(dialog, &KConfigDialog::settingsChanged, mpGameView, &KBlocksView::settingsChanged);
    //connect(dialog, SIGNAL(hidden()), view, SLOT(resumeFromConfigure()));
    dialog->show();
}

void KBlocksWin::onScoreChanged(int index, int points, int lines, int level)
{
    if (index == 0) { // TODO : game id?? multi game display??
        mScore->setText(i18n("Points: %1 - Lines: %2 - Level: %3", points, lines, level));
    }
}

void KBlocksWin::onIsHighscore(int index, int points, int level)
{
    if (index == 0) { // TODO : game id?? multi game display??
        QPointer<KScoreDialog> ksdialog = new KScoreDialog(KScoreDialog::Name | KScoreDialog::Level | KScoreDialog::Score, this);
        ksdialog->initFromDifficulty(Kg::difficulty());
        KScoreDialog::FieldInfo info;
        info[KScoreDialog::Score].setNum(points);
        info[KScoreDialog::Level].setNum(level);
        if (ksdialog->addScore(info)) {
            ksdialog->exec();
        }
        delete ksdialog;
    }
}

void KBlocksWin::levelChanged()
{
    //Scene reads the difficulty level for us
    if (mGameCount == 1) {
        singleGame();
    } else {
        pveStepGame();
    }
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
    actionCollection()->addAction(QStringLiteral("newGame"), action);

    action = new QAction(this);
    action->setText(i18n("Human vs AI"));
    actionCollection()->addAction(QStringLiteral("pve_step"), action);
    connect(action, &QAction::triggered, this, &KBlocksWin::pveStepGame);

    m_pauseAction = KStandardGameAction::pause(this, SLOT(pauseGame()), actionCollection());
    actionCollection()->addAction(QStringLiteral("pauseGame"), m_pauseAction);
    m_pauseAction->setEnabled(false);

    action = KStandardGameAction::highscores(this, SLOT(showHighscore()), actionCollection());
    actionCollection()->addAction(QStringLiteral("showHighscores"), action);

    action = KStandardGameAction::quit(this, SLOT(close()), actionCollection());
    actionCollection()->addAction(QStringLiteral("quit"), action);

    KStandardAction::preferences(this, SLOT(configureSettings()), actionCollection());

    KToggleAction *soundAction = new KToggleAction(i18n("&Play Sounds"), this);
    soundAction->setChecked(Settings::sounds());
    actionCollection()->addAction(QStringLiteral("sounds"), soundAction);
    connect(soundAction, &KToggleAction::triggered, this, &KBlocksWin::setSoundsEnabled);
    mpGameScene->setSoundsEnabled(Settings::sounds());

    // TODO
    mScore = new QLabel(i18n("Points: 0 - Lines: 0 - Level: 0"));
    statusBar()->addPermanentWidget(mScore);
    connect(mpGameScene, &KBlocksScene::scoreChanged, this, &KBlocksWin::onScoreChanged);
    connect(mpGameScene, &KBlocksScene::isHighscore, this, &KBlocksWin::onIsHighscore);

    Kg::difficulty()->addStandardLevelRange(
        KgDifficultyLevel::Easy, KgDifficultyLevel::Hard
    );
    KgDifficultyGUI::init(this);
    connect(Kg::difficulty(), &KgDifficulty::currentLevelChanged, this, &KBlocksWin::levelChanged);

    setupGUI();
}

