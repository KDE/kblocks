#ifndef KBLOCKSSINGLEGAME_H
#define KBLOCKSSINGLEGAME_H

#include "SingleGameInterface.h"

#include "KBlocksField.h"
#include "KBlocksPiece.h"
#include "KBlocksPieceGenerator.h"

#include "KBlocksDefine.h"

class KBlocksSingleGame : public SingleGameInterface {
	public:
		KBlocksSingleGame(int showPieceCount = 2);
		~KBlocksSingleGame();
        
    public:
		KBlocksField* getField();
        
        int getPieceCount();
        KBlocksPiece* getPiece(int index);
        
		bool setCurrentPiece(int xPos, int yPos, int rotation);
        
        bool pushGameAction(int type, int param);
        
        bool popGameAction(int type, int * param);
        void clearGameActions();
		
		int startGame(int seed);
        int pauseGame();
        int resumeGame();
        int stopGame();
        
        int runStep();
        int runEvent(int gameEvent);
        
        int getLastGameResult();
        
    private:
        bool checkPieceTouchGround(KBlocksPiece * p);
        void freezePieceToField(KBlocksPiece * p);
        bool removeFieldLines();
        void prepareNextPiece();
        
    protected:
        KBlocksField* mpField;
        
        int mPieceCount;
        KBlocksPiece** mpPieceList;
        
    private:
        KBlocksPieceGenerator* mpPieceGenerator;
        
        int mActionIndex;
        int* maActionType;
        int* maActionParam;
        
        int mLastGameResult;
        int mCurrentGameState;
};

#endif
