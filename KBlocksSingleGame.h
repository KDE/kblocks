/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

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
