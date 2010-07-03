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
#include <time.h>
#include <string>
#include <vector>

#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kglobal.h>

#include <QString>
#include <QList>
#include <QByteArray>
#include <string>
#include <time.h>

using namespace std;

#include "KBlocksConfigManager.h"

#include "KBlocksGameLogic.h"
#include "KBlocksPlayManager.h"
#include "KBlocksPlayNetwork.h"
#include "KBlocksWin.h"
#include "KBlocksDisplay.h"
#include "KBlocksRepWin.h"

#include "KBlocksAppThread.h"

#include "KBlocksPlayNetwork.h"
#include "AI/KBlocksAIPlayer.h"
#include "KBlocksKeyboardPlayer.h"

#include "KBlocksNetServer.h"
#include "KBlocksNetClient.h"

KBlocksGameLogic* mpKBlocksGameLogic;
KBlocksPlayManager* mpKBlocksPlayManager;
KBlocksPlayNetwork* mpKBlocksPlayNetwork;
KBlocksWin* mpKBlocksWindow;
KBlocksDisplay* mpKBlocksDisplay;
KBlocksAppThread* mpKBlocksAppThread;

KBlocksAIPlayer** maAIPlayers;
KBlocksKeyboardPlayer** maHumanPlayers;

enum KBlocksGameMode
{
    KBlocksGame_DesktopMode = 0,
    KBlocksGame_EngineMode,
    KBlocksGame_GuiMode,
    KBlocksGame_PlayerMode,
    KBlocksGame_ReplayMode,
    KBlocksGame_MaxMode_Count
};

int gameDesktopMode(const KApplication& app)
{
    // Desktop User Mode
    mpKBlocksGameLogic = new KBlocksGameLogic(2);
    mpKBlocksGameLogic->setGameSeed(time(0));
    mpKBlocksGameLogic->setGamePunish(true);
    mpKBlocksGameLogic->setGameStandbyMode(true);
    mpKBlocksGameLogic->setInitInterval(500);
    mpKBlocksGameLogic->setLevelUpInterval(25);
    
    mpKBlocksPlayManager = new KBlocksPlayManager(mpKBlocksGameLogic, 2);
    
    mpKBlocksWindow = new KBlocksWin(mpKBlocksGameLogic, mpKBlocksPlayManager, 2, 1);
    mpKBlocksWindow->setUpdateInterval(50);
    mpKBlocksWindow->setGamesPerLine(4);
    mpKBlocksWindow->setGameAnimEnabled(true);
    mpKBlocksWindow->setWaitForAllUpdate(true);
    mpKBlocksWindow->show();
    
    return app.exec();
}

int gameEngineMode(KBlocksConfigManager * config)
{
    int gameCount;
    bool sameSeq;
    bool hasAttack;
    bool standbyMode;
    bool hasHuman;
    bool sendLimit;
    string serverIP;
    
    string recordFile;
    string recordType;
    bool recordBinary;
    
    config->GetKeyInt("Engine", "GameCount", &gameCount, 1);
    config->GetKeyBool("Engine", "SameSequence", &sameSeq, true);
    config->GetKeyBool("Engine", "HasAttack", &hasAttack, true);
    config->GetKeyBool("Engine", "Synchronized", &standbyMode, false);
    config->GetKeyBool("Engine", "HasHuman", &hasHuman, false);
    config->GetKeyBool("Engine", "SendLimit", &sendLimit, false);
    config->GetKeyString("Engine", "ServerIP", &serverIP, "127.0.0.1:10086");
    
    config->GetKeyString("RecordReplay", "Record", &recordFile, "");
    config->GetKeyString("RecordReplay", "Type", &recordType, "binary");
    recordBinary = recordType.find("text") != 0;
    
    printf("Creating game engine...\n");
    printf("\tGame Count    = %d\n", gameCount);
    printf("\tSame Sequence = %s\n", sameSeq ? "true" : "false");
    printf("\tHas Attack    = %s\n", hasAttack ? "true" : "false");
    printf("\tSynchronized  = %s\n", standbyMode ? "true" : "false");
    printf("\tHas Human     = %s\n", hasHuman ? "true" : "false");
    printf("\tSpeed Limit   = %s\n", sendLimit ? "true" : "false");
    mpKBlocksGameLogic = new KBlocksGameLogic(gameCount, true);
    mpKBlocksGameLogic->setGameSeed(sameSeq ? time(0) : -time(0));
    mpKBlocksGameLogic->setGamePunish(hasAttack);
    mpKBlocksGameLogic->setGameStandbyMode(standbyMode);
    mpKBlocksGameLogic->setInitInterval(hasHuman ? 500 : 0);
    mpKBlocksGameLogic->setLevelUpInterval(hasHuman ? 25 : 0);
    printf("Done...\n");
    
    printf("Creating network server...\n");
    printf("\tServer IP = %s\n", serverIP.c_str());
    printf("\tRecord File = %s\n", recordFile.c_str());
    printf("\tRecord Type = %s\n", recordBinary ? "Binary" : "Text");
    KBlocksNetServer* mpKBlocksServer = new KBlocksNetServer(mpKBlocksGameLogic, serverIP.c_str());
    mpKBlocksServer->setSendLength(sendLimit ? 10 : 0, sendLimit ? 1 : 0);
    mpKBlocksServer->setRecordFile(recordFile.c_str(), recordBinary);
    printf("Done...\n");
    
    printf("Executing game engine and network server...\n");
    return mpKBlocksServer->executeGame(gameCount, standbyMode);
}

int gameGuiMode(KBlocksConfigManager * config, const KApplication& app)
{
    int gameCount;
    int gamesPerLine;
    int updateInterval;
    int localPort;
    string serverIP;
    
    config->GetKeyInt("Gui", "GameCount", &gameCount, 1);
    config->GetKeyInt("Gui", "GamesPerLine", &gamesPerLine, 4);
    config->GetKeyInt("Gui", "UpdateInterval", &updateInterval, 1000);
    config->GetKeyInt("Gui", "LocalPort", &localPort, 10088);
    config->GetKeyString("Gui", "ServerIP", &serverIP, "127.0.0.1:10086");
    
    printf("Creating game gui...\n");
    printf("\tGame Count      = %d\n", gameCount);
    printf("\tGames Per Line  = %d\n", gamesPerLine);
    printf("\tUpdate Interval = %d\n", updateInterval);
    printf("\tLocal Port      = %d\n", localPort);
    printf("\tServer IP       = %s\n", serverIP.c_str());
    mpKBlocksDisplay = new KBlocksDisplay(gameCount, serverIP, localPort);
    mpKBlocksDisplay->setGamesPerLine(gamesPerLine);
    mpKBlocksDisplay->setUpdateInterval(updateInterval);
    mpKBlocksDisplay->show();
    printf("Done...\n");
    
    printf("Starting game gui...\n");
    mpKBlocksDisplay->startDisplay();
    printf("Done...\n");
    
    return app.exec();
}

int gameReplayMode(KBlocksConfigManager * config, const QApplication& app)
{
    int gamesPerLine;
    int updateInterval;
    int stepLength;
    string snapshotFolder;
    string snapshotFile;
    string recordFile;
    string recordType;
    bool recordBinary;
    
    config->GetKeyInt("RecordReplay", "GamesPerLine", &gamesPerLine, 4);
    config->GetKeyInt("RecordReplay", "UpdateInterval", &updateInterval, 100);
    config->GetKeyInt("RecordReplay", "StepLength", &stepLength, 100);
    config->GetKeyString("RecordReplay", "SnapshotFolder", &snapshotFolder, "./snapshot/");
    config->GetKeyString("RecordReplay", "SnapshotFile", &snapshotFile, "");
    config->GetKeyString("RecordReplay", "Record", &recordFile, "");
    config->GetKeyString("RecordReplay", "Type", &recordType, "binary");
    recordBinary = recordType.find("text") != 0;
    
    if (recordFile.empty())
    {
        printf("Error loading replay file: File name is empty!\n");
        return -1;
    }
    
    printf("Creating game gui...\n");
    printf("\tGames Per Line  = %d\n", gamesPerLine);
    printf("\tUpdate Interval = %d\n", updateInterval);
    printf("\tStep Length     = %d\n", stepLength);
    printf("\tSnapshot Folder = %s\n", snapshotFolder.c_str());
    printf("\tSnapshot File   = %s\n", snapshotFile.c_str());
    printf("\tRecord File     = %s\n", recordFile.c_str());
    printf("\tRecord Type     = %s\n", recordBinary ? "Binary" : "Text");
    
    KBlocksRepWin* mpKBlocksRepWin = new KBlocksRepWin(recordFile.c_str(), recordBinary);
    if (!mpKBlocksRepWin->replayLoaded())
    {
        printf("Error loading replay file: Failed to load replay file!\n");
        return -2;
    }
    mpKBlocksRepWin->setGamesPerLine(gamesPerLine);
    mpKBlocksRepWin->setUpdateInterval(updateInterval);
    mpKBlocksRepWin->setReplayStepLength(stepLength);
    mpKBlocksRepWin->setSnapshotFolder(snapshotFolder.c_str());
    mpKBlocksRepWin->setSnapshotFilename(snapshotFile.c_str());
    mpKBlocksRepWin->show();
    printf("Done...\n");
    
    printf("Starting game gui...\n");
    mpKBlocksRepWin->startReplay();
    printf("Done...\n");
    
    return app.exec();
}

int gamePlayerMode(KBlocksConfigManager * config, const KApplication& app)
{
    bool hasHuman = false;
    int playerCount;
    int localPort;
    string serverIP;
    
    config->GetKeyInt("Player", "PlayerCount", &playerCount, 1);
    config->GetKeyInt("Player", "LocalPort", &localPort, 10090);
    config->GetKeyString("Player", "ServerIP", &serverIP, "127.0.0.1:10086");
    
    printf("Creating game player manager...\n");
    printf("\tPlayer Count = %d\n", playerCount);
    printf("\tLocal Port   = %d\n", localPort);
    printf("\tServer IP    = %s\n", serverIP.c_str());
    mpKBlocksPlayNetwork = new KBlocksPlayNetwork(playerCount, serverIP, localPort);
    printf("Done...\n");
    
    printf("Adding game players...\n");
    maAIPlayers = new KBlocksAIPlayer*[playerCount];
    maHumanPlayers = new KBlocksKeyboardPlayer*[playerCount];
    for(int i = 0; i < playerCount; i++)
    {
        maAIPlayers[i] = 0;
        maHumanPlayers[i] = 0;
        
        char tmpBuf[256];
        sprintf( tmpBuf, "PlayerType%d", i+1 );
        string tmpType = string(tmpBuf);
        sprintf( tmpBuf, "PlayerName%d", i+1 );
        string tmpName = string(tmpBuf);
        config->GetKeyString("Player", tmpType, &tmpType, "AI");
        config->GetKeyString("Player", tmpName, &tmpName, "NoName");
        
        printf("\tNew Player (%d) Type = %s\n", i, tmpType.c_str());
        printf("\t               Name = %s\n", tmpName.c_str());
        if (tmpType.find("ai") != tmpType.npos)
        {
            maAIPlayers[i] = new KBlocksAIPlayer(tmpName);
            mpKBlocksPlayNetwork->addGamePlayer(maAIPlayers[i]);
        }
        else if (tmpType.find("human") != tmpType.npos)
        {
            maHumanPlayers[i] = new KBlocksKeyboardPlayer(NULL, tmpName, true);
            mpKBlocksPlayNetwork->addGamePlayer(maHumanPlayers[i]);
            hasHuman = true;
        }
    }
    printf("Done...\n");
    
    printf("Starting play manager...\n");
    mpKBlocksPlayNetwork->startGame();
    
    int ret = 0;
    if (hasHuman)
    {
        mpKBlocksAppThread = new KBlocksAppThread(mpKBlocksPlayNetwork);
        printf("Executing play manager...\n");
        mpKBlocksAppThread->start();
        
        printf("Executing keyboard window...\n");
        ret = app.exec();
        
        printf("Terminating play manager execution...\n");
        mpKBlocksPlayNetwork->cancelExecute();
    }
    else
    {
        printf("Executing...\n");
        ret = mpKBlocksPlayNetwork->execute();
    }
    
    printf("Stopping play manager...\n");
    mpKBlocksPlayNetwork->stopGame();
    
    printf("Clearing game players...\n");
    mpKBlocksPlayNetwork->clearGamePlayer();
    
    printf("Exit program...\n");
    return ret;
}

int main (int argc, char *argv[])
{
    // Game abouts...
    KAboutData aboutData( "kblocks", 0,
                          ki18n("KBlocks"),
                          "0.3",
                          ki18n("A falling blocks game for KDE"),
                          KAboutData::License_GPL,
                          ki18n("(c) 2007, Mauricio Piacentini") );
    aboutData.addAuthor(ki18n("Mauricio Piacentini"), ki18n("Author"), "piacentini@kde.org");
    aboutData.addAuthor(ki18n("Dirk Leifeld"), ki18n("Developer"), "dirkleifeld@yahoo.de");
    aboutData.addAuthor(ki18n("Zhongjie Cai"), ki18n("New design of KBlocks for AI and tetris research platform"), "squall.leonhart.cai@gmail.com");
    aboutData.addCredit(ki18n("Johann Ollivier Lapeyre"), ki18n("Oxygen art for KDE4"), "johann.ollivierlapeyre@gmail.com");
    
    // Command line argument options
    KCmdLineOptions options;
    
    options.add("mode <game mode>", ki18n("Setup kblocks game running mode.\n\t0 = Desktop Mode\t1 = Game Engine Mode\n\t2 = Gui Mode\t3 = Player Mode"), "0");
    options.add("conf <configuration file>", ki18n("Setup the configuration file for tetris researcher mode. Not for desktop users."), "default.conf");
    
    KCmdLineArgs::init( argc, argv, &aboutData );
    KCmdLineArgs::addCmdLineOptions( options );
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    
    // Application
    KApplication app;
    KGlobal::locale()->insertCatalog("libkdegames");
    
    // Get game mode
    int mGameMode = args->getOption("mode").toInt();
    
    QByteArray tmpFileArray = args->getOption("conf").toLatin1();
    const char *tmpFileChar = tmpFileArray.data();
    KBlocksConfigManager* config = new KBlocksConfigManager();
    config->LoadConfigFile(string(tmpFileChar));
    
    int mResult = 0;
    switch(mGameMode)
    {
        case KBlocksGame_DesktopMode:
            mResult = gameDesktopMode(app);
            break;
        case KBlocksGame_EngineMode:
            mResult = gameEngineMode(config);
            break;
        case KBlocksGame_GuiMode:
            mResult = gameGuiMode(config, app);
            break;
        case KBlocksGame_PlayerMode:
            mResult = gamePlayerMode(config, app);
            break;
        case KBlocksGame_ReplayMode:
            mResult = gameReplayMode(config, app);
            break;
    }
    
    return mResult;
}

