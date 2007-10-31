

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
#include <KStandardGameAction>
#include <KStandardAction>
#include <KIcon>
#include <KScoreDialog>
#include <KGameThemeSelector>
#include <KLocale>
#include <KToggleAction>
#include <KActionCollection>

#include <QPixmapCache>

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
    
    QAction *action = KStandardGameAction::gameNew(view, SLOT(newGame()), actionCollection());
    actionCollection()->addAction("newGame", action);
    
    action = KStandardGameAction::pause(view, SLOT(pauseGame()), actionCollection());
    actionCollection()->addAction("pauseGame", action);

    action = KStandardGameAction::quit(this, SLOT(close()), actionCollection());
    actionCollection()->addAction("quit", action);
    
    KStandardAction::preferences(this, SLOT(configureSettings()), actionCollection());
    
    setupGUI();
}

KBlocks::~KBlocks()
{
    delete view;
}

void KBlocks::configureSettings()
{
  if ( KConfigDialog::showDialog("settings") ) return;

  KConfigDialog *dialog = new KConfigDialog(this, "settings", Settings::self());
  dialog->addPage(new KGameThemeSelector(dialog, Settings::self()), i18n("Theme"), "game_theme");
  connect(dialog, SIGNAL(settingsChanged(const QString &)), view, SLOT(settingsChanged()));
  dialog->show();
}


#include "kblocks.moc"
