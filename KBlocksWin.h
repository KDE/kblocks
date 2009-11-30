#ifndef KBLOCKSWIN_H
#define KBLOCKSWIN_H

#include <KXmlGuiWindow>
#include <KGameDifficulty>

#include "KBlocksView.h"
#include "KBlocksScene.h"

#include "GameLogicInterface.h"

class KBlocksWin : public KXmlGuiWindow
{
    Q_OBJECT

    public:
        KBlocksWin(GameLogicInterface * p);
        ~KBlocksWin();
        
    private slots:
        void newGame();
        void endGame();
        void configureSettings();
        void pauseGame();
        void showHighscore();
        void onScoreChanged(int points, int lines, int level);
        void onIsHighscore(int points, int level);
        void levelChanged(KGameDifficulty::standardLevel);
        void setSoundsEnabled(bool enabled);
        
    private:
        void setupKeyboard();
        void setupGUILayout();

    protected:
        virtual bool queryExit();
        
    private:
        int mMaxGameCapacity;
        int mGameCount;
        
        KBlocksView*  mpGameView;
        KBlocksScene* mpGameScene;
        KBlocksEvent* mpEventHandler;
        
        GameLogicInterface* mpLocalGameLogic;
        
        QAction*  m_pauseAction;
};

#endif
