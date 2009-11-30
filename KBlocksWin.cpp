#include "KBlocksWin.h"

#include <limits.h>

#include <KAction>
#include <KConfigDialog>
#include <kstandardgameaction.h>
#include <KStandardAction>
#include <KIcon>
#include <kscoredialog.h>
#include <KGameThemeSelector>
#include <KLocale>
#include <KToggleAction>
#include <KActionCollection>
#include <KStatusBar>

#include <QPixmapCache>

#include <time.h>

#include "settings.h"

KBlocksWin::KBlocksWin(GameLogicInterface * p) : KXmlGuiWindow()
{
    //Use up to 3MB for global application pixmap cache
    QPixmapCache::setCacheLimit(3*1024);
    
    mpLocalGameLogic = p;
    mpLocalGameLogic->setGameSeed(0);
    
    int capacity = mpLocalGameLogic->getGameMaxCapacity();
    
    mpGameScene = new KBlocksScene(capacity);
    mpEventHandler = new KBlocksEvent();
    
    mpGameScene->setEventHandler(mpEventHandler);
    mpEventHandler->setGameLogic(mpLocalGameLogic);
    
    mpGameView = new KBlocksView(this);
    mpGameView->setGameScene(mpGameScene);
	mpGameView->show();
    setCentralWidget(mpGameView);
    
    mMaxGameCapacity = capacity;
    mGameCount = capacity;
    
    qsrand(time(0));
    setAutoSaveSettings();
    
    setupGUILayout();
}

KBlocksWin::~KBlocksWin()
{
    delete mpGameView;
    delete mpLocalGameLogic;
    delete mpEventHandler;
    delete mpGameScene;
}

void KBlocksWin::newGame()
{
    if (mpGameScene->getGameState() != GameState_Stop)
    {
        endGame();
    }
    
    mpLocalGameLogic->createSingleGames(mGameCount);
    mpGameScene->createGameItemGroups(mGameCount, mpLocalGameLogic);
    
    setupKeyboard();
    
    mpGameScene->startGame();
}

void KBlocksWin::endGame()
{
    if (mpGameScene->getGameState() != GameState_Stop)
    {
        mpGameScene->stopGame();
        
        mpGameScene->deleteGameItemGroups();
        mpLocalGameLogic->deleteSingleGames();
    }
}

bool KBlocksWin::queryExit()
{
    Settings::self()->writeConfig();
    return true;
}

void KBlocksWin::pauseGame()
{
    mpGameScene->pauseGame(m_pauseAction->isChecked(), true);
}

void KBlocksWin::showHighscore()
{
    KScoreDialog ksdialog(KScoreDialog::Name | KScoreDialog::Level | KScoreDialog::Score, this);
    ksdialog.setConfigGroup(KGameDifficulty::localizedLevelString());
    ksdialog.exec();
}

void KBlocksWin::configureSettings()
{
    if ( KConfigDialog::showDialog("settings") ) {
        return;
    }
    KConfigDialog *dialog = new KConfigDialog(this, "settings", Settings::self());
    dialog->addPage(new KGameThemeSelector(dialog, Settings::self()), i18n("Theme"), "games-config-theme");
    connect(dialog, SIGNAL(settingsChanged(const QString &)), mpGameView, SLOT(settingsChanged()));
    dialog->show();
}

void KBlocksWin::onScoreChanged(int points, int lines, int level)
{
    statusBar()->changeItem( i18n("Points: %1 - Lines: %2 - Level: %3", points, lines, level), 0 );
}

void KBlocksWin::onIsHighscore(int points, int level)
{
    KScoreDialog ksdialog( KScoreDialog::Name | KScoreDialog::Level | KScoreDialog::Score, this );
    ksdialog.setConfigGroup(KGameDifficulty::localizedLevelString());
    KScoreDialog::FieldInfo info;
    info[KScoreDialog::Score].setNum( points );
    info[KScoreDialog::Level].setNum( level );
    if ( ksdialog.addScore( info ) )
    {
        ksdialog.exec();
    }
}

void KBlocksWin::levelChanged(KGameDifficulty::standardLevel)
{
    //Scene reads the difficulty level for us
    newGame();
    mpGameView->setFocus(Qt::MouseFocusReason);
}

void KBlocksWin::setSoundsEnabled(bool enabled)
{
    mpGameScene->setSoundsEnabled(enabled);
    Settings::setSounds(enabled);
}

void KBlocksWin::setupKeyboard()
{
    // TODO : Read settings and setup keyboard
    
    KAction* rotatecw = actionCollection()->addAction("rotate_cw");
    rotatecw->setText(i18n("Rotate Piece Clockwise"));
    rotatecw->setIcon(KIcon("object-rotate-right"));
    rotatecw->setShortcuts( KShortcut( "z" ) );
    
    KAction* rotateccw = actionCollection()->addAction("rotate_ccw");
    rotateccw->setText(i18n("Rotate Piece Counter Clockwise"));
    rotateccw->setIcon(KIcon("object-rotate-left"));
    rotateccw->setShortcuts( KShortcut( Qt::Key_Up  ) );
    
    KAction* moveleft = actionCollection()->addAction("move_left");
    moveleft->setText(i18n("Move Piece Left"));
    moveleft->setIcon(KIcon("arrow-left"));
    moveleft->setShortcuts( KShortcut( Qt::Key_Left ) );
    
    KAction* moveright = actionCollection()->addAction("move_right");
    moveright->setText(i18n("Move Piece Right"));
    moveright->setIcon(KIcon("arrow-right"));
    moveright->setShortcuts( KShortcut( Qt::Key_Right) );
    
    KAction* movedown = actionCollection()->addAction("move_down");
    movedown->setText(i18n("Move Piece Down"));
    movedown->setIcon(KIcon("arrow-down"));
    movedown->setShortcuts( KShortcut( Qt::Key_Down ) );
    
    KAction* pushdown = actionCollection()->addAction("push_down");
    pushdown->setText(i18n("Drop the Piece"));
    pushdown->setIcon(KIcon("arrow-down"));
    pushdown->setShortcuts( KShortcut( Qt::Key_Space ) );
        
    for(int i = 0; i < mGameCount; i++)
    {
        connect(rotatecw, SIGNAL(triggered(bool)), mpGameScene->getItemGroup(i), SLOT(rotateCW()));
        connect(rotateccw, SIGNAL(triggered(bool)), mpGameScene->getItemGroup(i), SLOT(rotateCCW()));
        connect(moveleft, SIGNAL(triggered(bool)), mpGameScene->getItemGroup(i), SLOT(moveLeft()));
        connect(moveright, SIGNAL(triggered(bool)), mpGameScene->getItemGroup(i), SLOT(moveRight()));
        connect(movedown, SIGNAL(triggered(bool)), mpGameScene->getItemGroup(i), SLOT(moveDown()));
        connect(pushdown, SIGNAL(triggered(bool)), mpGameScene->getItemGroup(i), SLOT(pushDown()));
    }
}

void KBlocksWin::setupGUILayout()
{
    QAction *action = KStandardGameAction::gameNew(this, SLOT(newGame()), actionCollection());
    actionCollection()->addAction("newGame", action);
    
    m_pauseAction = KStandardGameAction::pause(this, SLOT(pauseGame()), actionCollection());
    actionCollection()->addAction("pauseGame", m_pauseAction);
    
    action = KStandardGameAction::highscores(this, SLOT(showHighscore()), actionCollection());
    actionCollection()->addAction("showHighscores", action);
    
    action = KStandardGameAction::quit(this, SLOT(close()), actionCollection());
    actionCollection()->addAction("quit", action);
    
    KStandardAction::preferences(this, SLOT(configureSettings()), actionCollection());

    //restore difficulty and sound from settings, need to read them first
    Settings::self()->readConfig();

    KAction* soundAction = new KToggleAction(i18n("&Play sounds"), this);
    soundAction->setChecked(Settings::sounds());
    actionCollection()->addAction("sounds", soundAction);
    connect(soundAction, SIGNAL(triggered(bool)), this, SLOT(setSoundsEnabled(bool)));
    
    // TODO
    statusBar()->insertItem( i18n("Points: 0 - Lines: 0 - Level: 0"), 0 );
    connect(mpGameScene, SIGNAL(scoreChanged(int, int, int)), this,  SLOT(onScoreChanged(int, int, int)));
    connect(mpGameScene, SIGNAL(isHighscore(int, int)), this,  SLOT(onIsHighscore(int, int)));
    
    KGameDifficulty::init(this, this, SLOT(levelChanged(KGameDifficulty::standardLevel)));
    KGameDifficulty::setRestartOnChange(KGameDifficulty::RestartOnChange);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Easy);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Medium);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Hard);
    
    int difficulty = Settings::difficulty();
    if ((difficulty < KGameDifficulty::Easy) || (difficulty > KGameDifficulty::Hard) )
    {
      //unexpected, but use a default
      KGameDifficulty::setLevel(KGameDifficulty::Easy);
    }
    else
    {
      KGameDifficulty::setLevel((KGameDifficulty::standardLevel) (difficulty));
    }
    
    setupGUI();
}

