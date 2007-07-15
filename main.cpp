/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2007 Mauricio Piacentini <mauricio@tabuleiro.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kglobal.h>

#include "kblocks.h"

int main( int argc, char** argv )
{
  KAboutData aboutData( "kblocks", 0, ki18n("KBlocks"),
                        "0.1", ki18n("A falling blocks game for KDE"), KAboutData::License_GPL,
                            ki18n("(c) 2007, Mauricio Piacentini"));
  aboutData.addAuthor(ki18n("Mauricio Piacentini"), ki18n("Author"), "mauricio@tabuleiro.com");
  aboutData.addCredit(ki18n("Johann Ollivier Lapeyre"), ki18n("Oxygen art for KDE4"), "johann.ollivierlapeyre@gmail.com");

  KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication app;
  KGlobal::locale()->insertCatalog("libkdegames");
 
  KBlocks *window = new KBlocks();
  window->show();
  return app.exec();
}
