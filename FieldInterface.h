/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

/****************************************************************
 *
 * $RCSfile: FieldInterface.h,v $
 *
 * $Revision: 1.0 $
 *
 * $Date: 2009/11/01 10:00:00 $
 *
 * AUTHOR: ZhongJie Cai
 *
 * DESCRIPTION: Interface for game board of KBlocks 
 *
 ****************************************************************/  
#ifndef FIELD_INTERFACE
#define FIELD_INTERFACE

class FieldInterface {
    protected:
        bool** board;
        int height;
        int width;
    
    public:
        FieldInterface(){};
        virtual ~FieldInterface(){};
    
    public:
        virtual bool getCell(int xPos, int yPos) = 0;
        virtual void setCell(int xPos, int yPos, bool value) = 0;
        
        virtual int  getWidth() = 0;
        virtual int  getHeight() = 0;
};

#endif //FIELD_INTERFACE
