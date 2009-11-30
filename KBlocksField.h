#ifndef KBLOCKSFIELD_H
#define KBLOCKSFIELD_H

#include "FieldInterface.h"

class KBlocksField : public FieldInterface {
    protected:
        bool** board;
        int height;
        int width;
    
    public:
        KBlocksField(int w = 10, int h = 20);
        ~KBlocksField();
    
    public:
        bool getCell(int xPos, int yPos);
        void setCell(int xPos, int yPos, bool value);
        
        void clearAll();
        
        bool checkFilledLine(int lineID);
        void removeFilledLine(int lineID);
        
        bool addPunishLine(int emptyID = -1);
        
        int  getWidth();
        int  getHeight();
};

#endif
