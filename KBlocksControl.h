#ifndef KBLOCKSCONTROL_H
#define KBLOCKSCONTROL_H

#include "SingleGameInterface.h"

#include "KBlocksDefine.h"

class KBlocksControl {
    public:
        KBlocksControl();
        ~KBlocksControl();
        
        void setSingleGame(SingleGameInterface * p);
        
    public:
        bool rotateCW();
        bool rotateCCW();
        bool moveLeft();
        bool moveRight();
        bool moveDown();
        bool pushDown();
        
        int getRemovedLines(int * lineList);
        int getRemovedCount();
        
        int getNewPiecePosX(int * posX);
        int getNewPiecePosY(int * posY);
        
    private:
        SingleGameInterface* mpSingleGame;
};

#endif
