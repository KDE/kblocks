/***************************************************************************
 *   KBlocks, a falling blocks game for KDE                                *
 *   Copyright (C) 2007 Mauricio Piacentini <mauricio@tabuleiro.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef _KBLOCKS_H
#define _KBLOCKS_H

#include <KXmlGuiWindow>
#include <KGameDifficulty>

#include "kblocksview.h"


/**
   @short  Class Description

   @author Mauricio Piacentini
*/
class KBlocks : public KXmlGuiWindow
{
    Q_OBJECT

    public:
    /**
     * Constructor @param parent */
      KBlocks();
    /**
     * Default Destructor */
      ~KBlocks();
  private slots:
      void configureSettings();
      void pauseGame();
      void showHighscore();
      void onScoreChanged(int points, int lines, int level);
      void onIsHighscore(int points, int level);
      void levelChanged(KGameDifficulty::standardLevel);
  protected:
      virtual bool queryExit();
    private:
      KBlocksView* view;
      QAction*  m_pauseAction;

};

#endif

