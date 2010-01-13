/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSFIELD_H
#define KBLOCKSFIELD_H

#include "FieldInterface.h"

class KBlocksField : public FieldInterface
{
    protected:
        bool** maBoard;
        int mHeight;
        int mWidth;
        
    private:
        int mCurModifyID;
        int mLastModifyID;
        unsigned char* maEncodeData;
    
    public:
        KBlocksField(int width, int height);
        KBlocksField(FieldInterface * p);
        ~KBlocksField();
    
    public:
        bool getCell(int xPos, int yPos);
        void setCell(int xPos, int yPos, bool value);
        
        void copy(FieldInterface * p);
        void clear();
        
        bool checkFilledLine(int lineID);
        void removeFilledLine(int lineID);
        
        bool addPunishLine(int lineCount, int punishSeed);
        
        int  getModifyID();
        int  encodeData(unsigned char * data);
        void decodeData(unsigned char * data);
        
        int  getWidth();
        int  getHeight();
};

#endif
