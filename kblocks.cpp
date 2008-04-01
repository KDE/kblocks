

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
#include <KDE/KStandardGameAction>
#include <KStandardAction>
#include <KIcon>
#include <KDE/KScoreDialog>
#include <KDE/KGameThemeSelector>
#include <KDE/KStatusBar>
#include <KLocale>
#include <KToggleAction>
#include <KActionCollection>

#include <QPixmapCache>
#include <QCloseEvent>

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
    
    QAction* rotatecw = actionCollection()->addAction("rotate_cw");
    rotatecw->setText(i18n("Rotate Piece Clockwise"));
    rotatecw->setIcon(KIcon("object-rotate-right"));
    rotatecw->setShortcuts( KShortcut( "z" ) );
    connect(rotatecw, SIGNAL(triggered(bool)), view, SLOT(rotateCW()));
    
    QAction* rotateccw = actionCollection()->addAction("rotate_ccw");
    rotateccw->setText(i18n("Rotate Piece Counter Clockwise"));
    rotateccw->setIcon(KIcon("object-rotate-left"));
    rotateccw->setShortcuts( KShortcut( Qt::Key_Up  ) );
    connect(rotateccw, SIGNAL(triggered(bool)), view, SLOT(rotateCCW()));
    
    QAction* moveleft = actionCollection()->addAction("move_left");
    moveleft->setText(i18n("Move Piece Left"));
    moveleft->setIcon(KIcon("arrow-left"));
    moveleft->setShortcuts( KShortcut( Qt::Key_Left ) );
    connect(moveleft, SIGNAL(triggered(bool)), view, SLOT(moveLeft()));
    
    QAction* moveright = actionCollection()->addAction("move_right");
    moveright->setText(i18n("Move Piece Right"));
    moveright->setIcon(KIcon("arrow-right"));
    moveright->setShortcuts( KShortcut( Qt::Key_Right) );
    connect(moveright, SIGNAL(triggered(bool)), view, SLOT(moveRight()));
    
    QAction* movedown = actionCollection()->addAction("move_down");
    movedown->setText(i18n("Move Piece Down"));
    movedown->setIcon(KIcon("arrow-down"));
    movedown->setShortcuts( KShortcut( Qt::Key_Down ) );
    connect(movedown, SIGNAL(triggered(bool)), view, SLOT(moveDown()));
    
    statusBar()->insertItem( i18n("Points: 0 - Lines: 0 - Level: 0"), 0 );
    connect(view->getSceneObject(), SIGNAL(scoreChanged(int,int,int)), this,  SLOT(onScoreChanged(int,int,int)));
    connect(view->getSceneObject(), SIGNAL(isHighscore(int,int)), this,  SLOT(onIsHighscore(int,int)));
    
    KGameDifficulty::init(this, this, SLOT(levelChanged(KGameDifficulty::standardLevel)));
    KGameDifficulty::setRestartOnChange(KGameDifficulty::RestartOnChange);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Easy);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Medium);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Hard);
    
    //restore difficulty from settings, need to read it first
    Settings::self()->readConfig();
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

void KBlocks::close()
{
  Settings::self()->writeConfig();
}

void KBlocks::closeEvent(QCloseEvent *event)
{
  close();
  event->accept();
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
    view->pauseToConfigure();
    return;
  }

  KConfigDialog *dialog = new KConfigDialog(this, "settings", Settings::self());
  dialog->addPage(new KGameThemeSelector(dialog, Settings::self()), i18n("Theme"), "game_theme");
  connect(dialog, SIGNAL(settingsChanged(const QString &)), view, SLOT(settingsChanged()));
  connect(dialog, SIGNAL(hidden()), view, SLOT(resumeFromConfigure()));
  view->pauseToConfigure();
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
}



#include "kblocks.moc"
