#ifndef KBLOCKSSCENE_H
#define KBLOCKSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <KLocale>
#include <KGamePopupItem>

#include "KBlocksSound.h"
#include "KBlocksGraphics.h"
#include "KBlocksItemGroup.h"

#include "KBlocksEvent.h"

#include "KBlocksGameLogic.h"

#include "KBlocksScore.h"

#include "KBlocksDefine.h"

#define  SCENE_GAMES_PER_WIDTH    4

class KBlocksScene : public QGraphicsScene
{
    Q_OBJECT
    
    public:
        KBlocksScene(int capacity = 1);
        ~KBlocksScene();
        
        void setEventHandler(KBlocksEvent * p);
        
        void createGameItemGroups(int groupCount, GameLogicInterface * p);
        void deleteGameItemGroups();
        
        KBlocksGraphics* getGraphicHandler();
        KBlocksSound* getSoundHandler();
        
        KBlocksItemGroup* getItemGroup(int index);
        KBlocksControl* getControlHandler(int index);
        
        void readSettings(const QSize & viewSize);
        void viewScaled(const QSize& newsize);
        void updateDimensions();
        void setSoundsEnabled(bool enabled);
        
        void startGame();
        void pauseGame(bool flag, bool fromUser);
        void stopGame();
        
        int getGameState();
        
    signals:
        void scoreChanged(int points, int lines, int level);
        void isHighscore(int points, int level);
        
    public slots:
        void runGameOneStep(int gameIndex);
        
    private slots:
        void greetPlayer();
        void gameOverPlayer();
        
    protected:
        void drawBackground(QPainter * painter, const QRectF & rect);
        
    private:
        KBlocksGraphics* mpGrafx;
        KBlocksSound* mpSnd;
        
        int mMaxCapacity;
        int mGroupCount;
        KBlocksItemGroup** maGroupList;
        KBlocksControl** maControlList;
        KBlocksScore** maGameScoreList;
        
        KGamePopupItem* mMessageBox;
        
        KBlocksEvent* mpGameEvent;
        
        int mCurrentGameState;
};

#endif
