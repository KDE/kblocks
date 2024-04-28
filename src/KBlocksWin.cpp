/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2009-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#include "KBlocksWin.h"

#include <limits.h>
// KDEGames
#include <KGameThemeSelector>
#include <KGameThemeProvider>
#include <KGameDifficulty>
#include <KGameHighScoreDialog>
#include <KGameStandardAction>
// KF
#include <KToggleAction>
#include <KStandardAction>
#include <KActionCollection>
#include <KLocalizedString>
// Qt
#include <QStatusBar>
#include <QPixmapCache>
#include <QPointer>
#include <QLabel>
#include <QRandomGenerator>

#include "GameLogicInterface.h"
#include "KBlocksScene.h"
#include "SceneInterface.h"
#include "settings.h"

KBlocksWin::KBlocksWin(
    GameLogicInterface *p,
    GraphicsInterface *graphics,
    SoundInterface *sound,
    KGameThemeProvider *themeProvider,
    KBlocksPlayManager *pM,
    int capacity,
    int gamecount
) : KXmlGuiWindow()
{
    if (themeProvider) {
        m_themeSelector = new KGameThemeSelector(themeProvider, KGameThemeSelector::EnableNewStuffDownload);
        connect(themeProvider, &KGameThemeProvider::currentThemeChanged, this, &KBlocksWin::onThemeChanged);
    }

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
    delete m_themeSelector;
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
        switch ((int) KGameDifficulty::globalLevel()) {
        case KGameDifficultyLevel::Medium:
            levelUpTime = 5;
            break;
        case KGameDifficultyLevel::Hard:
            levelUpTime = 10;
            break;
        }
        mpGameLogic->levelUpGame(levelUpTime);

        KGameDifficulty::global()->setGameRunning(true);
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

    KGameDifficulty::global()->setGameRunning(!m_pauseAction->isChecked());
}

void KBlocksWin::onAllGamesStopped()
{
    mpPlayManager->stopGame();
    m_pauseAction->setEnabled(false);
    KGameDifficulty::global()->setGameRunning(false);
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
    KGameHighScoreDialog ksdialog(KGameHighScoreDialog::Name | KGameHighScoreDialog::Level | KGameHighScoreDialog::Score, this);
    ksdialog.initFromDifficulty(KGameDifficulty::global());
    ksdialog.exec();
}

void KBlocksWin::configureSettings()
{
    m_themeSelector->showAsDialog();
}

void KBlocksWin::onThemeChanged(const KGameTheme *theme)
{
    // sync to settings store
    Settings::setTheme(QString::fromUtf8(theme->identifier()));
    // trigger update of resources, then display
    mpGameView->loadTheme(theme);
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
        QPointer<KGameHighScoreDialog> ksdialog = new KGameHighScoreDialog(KGameHighScoreDialog::Name | KGameHighScoreDialog::Level | KGameHighScoreDialog::Score, this);
        ksdialog->initFromDifficulty(KGameDifficulty::global());
        KGameHighScoreDialog::FieldInfo info;
        info[KGameHighScoreDialog::Score].setNum(points);
        info[KGameHighScoreDialog::Level].setNum(level);
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

    action = KGameStandardAction::gameNew(this, &KBlocksWin::singleGame, actionCollection());
    action->setText(i18nc("@action", "Single Game"));
    actionCollection()->addAction(QStringLiteral("newGame"), action);

    action = new QAction(this);
    action->setText(i18nc("@action", "Human vs AI"));
    actionCollection()->addAction(QStringLiteral("pve_step"), action);
    connect(action, &QAction::triggered, this, &KBlocksWin::pveStepGame);

    m_pauseAction = KGameStandardAction::pause(this, &KBlocksWin::pauseGame, actionCollection());
    actionCollection()->addAction(QStringLiteral("pauseGame"), m_pauseAction);
    m_pauseAction->setEnabled(false);

    action = KGameStandardAction::highscores(this, &KBlocksWin::showHighscore, actionCollection());
    actionCollection()->addAction(QStringLiteral("showHighscores"), action);

    action = KGameStandardAction::quit(this, &KBlocksWin::close, actionCollection());
    actionCollection()->addAction(QStringLiteral("quit"), action);

    if (m_themeSelector) {
        KStandardAction::preferences(this, &KBlocksWin::configureSettings, actionCollection());
    }

    auto *soundAction = new KToggleAction(QIcon::fromTheme(QStringLiteral("speaker")),
                                          i18nc("@option:check", "Play Sounds"), this);
    soundAction->setChecked(Settings::sounds());
    actionCollection()->addAction(QStringLiteral("sounds"), soundAction);
    connect(soundAction, &KToggleAction::triggered, this, &KBlocksWin::setSoundsEnabled);
    mpGameScene->setSoundsEnabled(Settings::sounds());

    // TODO
    mScore = new QLabel(i18n("Points: 0 - Lines: 0 - Level: 0"));
    statusBar()->addPermanentWidget(mScore);
    connect(mpGameScene, &KBlocksScene::scoreChanged, this, &KBlocksWin::onScoreChanged);
    connect(mpGameScene, &KBlocksScene::isHighscore, this, &KBlocksWin::onIsHighscore);

    KGameDifficulty::global()->addStandardLevelRange(
        KGameDifficultyLevel::Easy, KGameDifficultyLevel::Hard
    );
    KGameDifficultyGUI::init(this);
    connect(KGameDifficulty::global(), &KGameDifficulty::currentLevelChanged, this, &KBlocksWin::levelChanged);

    setupGUI();
}

#include "moc_KBlocksWin.cpp"
