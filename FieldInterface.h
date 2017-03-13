/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef FIELD_INTERFACE
#define FIELD_INTERFACE

class FieldInterface
{
protected:
    FieldInterface() : maBoard(nullptr), mHeight(0), mWidth(0) {};

public:
    virtual ~FieldInterface() {};

public:
    virtual bool getCell(int xPos, int yPos) = 0;

    virtual int  getWidth() = 0;
    virtual int  getHeight() = 0;

protected:
    bool **maBoard;
    int mHeight;
    int mWidth;
};

#endif //FIELD_INTERFACE
