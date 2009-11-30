#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kglobal.h>

#include "KBlocksGameLogic.h"
#include "KBlocksWin.h"

int main (int argc, char *argv[])
{
    KAboutData aboutData( "kblocks", 0,
                          ki18n("KBlocks"),
                          "0.1",
                          ki18n("A falling blocks game for KDE"),
                          KAboutData::License_GPL,
                          ki18n("(c) 2007, Mauricio Piacentini") );
    aboutData.addAuthor(ki18n("Mauricio Piacentini"), ki18n("Author"), "piacentini@kde.org");
    aboutData.addAuthor(ki18n("Dirk Leifeld"), ki18n("Developer"), "dirkleifeld@yahoo.de");
    aboutData.addAuthor(ki18n("Zhongjie Cai"), ki18n("New design of KBlocks for multi-players and test platforms"), "squall.leonhart.cai@gmail.com");
    aboutData.addCredit(ki18n("Johann Ollivier Lapeyre"), ki18n("Oxygen art for KDE4"), "johann.ollivierlapeyre@gmail.com");
    
    //KCmdLineOptions options;
    //options.add("g <gameCount>", ki18n("Setup the number of single games in one game scene, default as 1."), "1");
    //options.add("nogui", ki18n("Start the game without GUI. If this option is enabled, no visual components will be initiated."));
    
    KCmdLineArgs::init( argc, argv, &aboutData );
    //KCmdLineArgs::addCmdLineOptions( options );
    
    KApplication app;
    KGlobal::locale()->insertCatalog("libkdegames");
    
    int gameCount = 1;
    //KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    //bool optionReadFlag;
    //int tmpCount = args->getOption("g").toInt(&optionReadFlag);
    //if (optionReadFlag)
    //{
    //    gameCount = tmpCount;
    //}
    
    KBlocksGameLogic * mpKBlocksGameLogic = new KBlocksGameLogic(gameCount);
    
    KBlocksWin * mpKBlocksWindow = new KBlocksWin(mpKBlocksGameLogic);
    
    mpKBlocksWindow->show();
    
    return app.exec();
}
