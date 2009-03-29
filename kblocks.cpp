

#include "kblocks.h"

#include <limits.h>

/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2007 Mauricio Piacentini <mauricio@tabuleiro.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <KAboutData>
#include <KAction>
#include <KConfigDialog>
#include <KInputDialog>
#include <KMenuBar>
#include <KMessageBox>
#include <kstandardgameaction.h>
#include <KStandardAction>
#include <KIcon>
#include <kscoredialog.h>
#include <KGameThemeSelector>
#include <KStatusBar>
#include <KLocale>
#include <KToggleAction>
#include <KActionCollection>

#include <QPixmapCache>

#include <time.h>

#include "settings.h"

/**
    Constructor.
*/
KBlocks::KBlocks()
    : KXmlGuiWindow()
{
    //Use up to 3MB for global application pixmap cache
    QPixmapCache::setCacheLimit(3*1024);

    // init view 
    view = new KBlocksView( this );
    setCentralWidget( view );
    
    qsrand(time(0));
    setAutoSaveSettings();
    
    QAction *action = KStandardGameAction::gameNew(view, SLOT(newGame()), actionCollection());
    actionCollection()->addAction("newGame", action);
    
    m_pauseAction = KStandardGameAction::pause(this, SLOT(pauseGame()), actionCollection());
    actionCollection()->addAction("pauseGame", m_pauseAction);
    
    action = KStandardGameAction::highscores(this, SLOT(showHighscore()), actionCollection());
    actionCollection()->addAction("showHighscores", action);
    
    action = KStandardGameAction::quit(this, SLOT(close()), actionCollection());
    actionCollection()->addAction("quit", action);
    
    KStandardAction::preferences(this, SLOT(configureSettings()), actionCollection());
    
    KAction* rotatecw = actionCollection()->addAction("rotate_cw");
    rotatecw->setText(i18n("Rotate Piece Clockwise"));
    rotatecw->setIcon(KIcon("object-rotate-right"));
    rotatecw->setShortcuts( KShortcut( "z" ) );
    connect(rotatecw, SIGNAL(triggered(bool)), view, SLOT(rotateCW()));
    
    KAction* rotateccw = actionCollection()->addAction("rotate_ccw");
    rotateccw->setText(i18n("Rotate Piece Counter Clockwise"));
    rotateccw->setIcon(KIcon("object-rotate-left"));
    rotateccw->setShortcuts( KShortcut( Qt::Key_Up  ) );
    connect(rotateccw, SIGNAL(triggered(bool)), view, SLOT(rotateCCW()));
    
    KAction* moveleft = actionCollection()->addAction("move_left");
    moveleft->setText(i18n("Move Piece Left"));
    moveleft->setIcon(KIcon("arrow-left"));
    moveleft->setShortcuts( KShortcut( Qt::Key_Left ) );
    connect(moveleft, SIGNAL(triggered(bool)), view, SLOT(moveLeft()));
    
    KAction* moveright = actionCollection()->addAction("move_right");
    moveright->setText(i18n("Move Piece Right"));
    moveright->setIcon(KIcon("arrow-right"));
    moveright->setShortcuts( KShortcut( Qt::Key_Right) );
    connect(moveright, SIGNAL(triggered(bool)), view, SLOT(moveRight()));
    
    KAction* movedown = actionCollection()->addAction("move_down");
    movedown->setText(i18n("Move Piece Down"));
    movedown->setIcon(KIcon("arrow-down"));
    movedown->setShortcuts( KShortcut( Qt::Key_Down ) );
    connect(movedown, SIGNAL(triggered(bool)), view, SLOT(moveDown()));
    
    KAction* pushdown = actionCollection()->addAction("push_down");
    pushdown->setText(i18n("Drop the Piece"));
    pushdown->setIcon(KIcon("arrow-down"));
    pushdown->setShortcuts( KShortcut( Qt::Key_Space ) );
    connect(pushdown, SIGNAL(triggered(bool)), view, SLOT(pushDown()));

    //restore difficulty and sound from settings, need to read them first
    Settings::self()->readConfig();

    KAction* soundAction = new KToggleAction(i18n("&Play sounds"), this);
    soundAction->setChecked(Settings::sounds());
    actionCollection()->addAction("sounds", soundAction);
    connect(soundAction, SIGNAL(triggered(bool)), this, SLOT(setSoundsEnabled(bool)));
    
    statusBar()->insertItem( i18n("Points: 0 - Lines: 0 - Level: 0"), 0 );
    connect(view->getSceneObject(), SIGNAL(scoreChanged(int,int,int)), this,  SLOT(onScoreChanged(int,int,int)));
    connect(view->getSceneObject(), SIGNAL(isHighscore(int,int)), this,  SLOT(onIsHighscore(int,int)));
    
    KGameDifficulty::init(this, this, SLOT(levelChanged(KGameDifficulty::standardLevel)));
    KGameDifficulty::setRestartOnChange(KGameDifficulty::RestartOnChange);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Easy);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Medium);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Hard);
    
    int difficulty = Settings::difficulty();
    if ((difficulty < KGameDifficulty::Easy) || (difficulty > KGameDifficulty::Hard) )
      //unexpected, but use a default
      KGameDifficulty::setLevel(KGameDifficulty::Easy);
    else
      KGameDifficulty::setLevel((KGameDifficulty::standardLevel) (difficulty));
    
    setupGUI();
}

KBlocks::~KBlocks()
{
    delete view;
}

bool KBlocks::queryExit()
{
  Settings::self()->writeConfig();
  return true;
}

void KBlocks::pauseGame()
{
  view->pauseGame(m_pauseAction->isChecked());
}

void KBlocks::showHighscore()
{
  KScoreDialog ksdialog(KScoreDialog::Name | KScoreDialog::Level | KScoreDialog::Score, this);
  ksdialog.setConfigGroup(KGameDifficulty::levelString());
  ksdialog.exec();
}

void KBlocks::onIsHighscore(int points, int level)
{
  KScoreDialog ksdialog( KScoreDialog::Name | KScoreDialog::Level | KScoreDialog::Score, this );
  ksdialog.setConfigGroup(KGameDifficulty::levelString());
  KScoreDialog::FieldInfo info;
  info[KScoreDialog::Score].setNum( points );
  info[KScoreDialog::Level].setNum( level );
  if ( ksdialog.addScore( info ) )
    ksdialog.exec();
}

void KBlocks::configureSettings()
{
  if ( KConfigDialog::showDialog("settings") ) {
    return;
  }

  KConfigDialog *dialog = new KConfigDialog(this, "settings", Settings::self());
  dialog->addPage(new KGameThemeSelector(dialog, Settings::self()), i18n("Theme"), "games-config-theme");
  connect(dialog, SIGNAL(settingsChanged(const QString &)), view, SLOT(settingsChanged()));
  //connect(dialog, SIGNAL(hidden()), view, SLOT(resumeFromConfigure()));
  dialog->show();

}

void KBlocks::onScoreChanged(int points, int lines, int level)
{
  statusBar()->changeItem( i18n("Points: %1 - Lines: %2 - Level: %3", points, lines, level), 0 );
}

void KBlocks::levelChanged(KGameDifficulty::standardLevel)
{
  //Scene reads the difficulty level for us
  view->newGame();
  view->setFocus(Qt::MouseFocusReason);
}

void KBlocks::setSoundsEnabled(bool enabled) {
  view->setSoundsEnabled(enabled);
  Settings::setSounds(enabled);
}



#include "kblocks.moc"
