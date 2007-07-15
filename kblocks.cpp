

#include "kblocks.h"

#include <limits.h>

#include <kaboutdata.h>
#include <kaction.h>
#include <kconfigdialog.h>
#include <kinputdialog.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <kstandardgameaction.h>
/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2007 Mauricio Piacentini <mauricio@tabuleiro.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
#include <kstandardaction.h>
#include <kicon.h>
#include <KScoreDialog>

#include <QPixmapCache>


#include <klocale.h>
#include <ktoggleaction.h>
#include <kactioncollection.h>

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
    
    setupGUI();
}

KBlocks::~KBlocks()
{
    delete view;
}

#include "kblocks.moc"
