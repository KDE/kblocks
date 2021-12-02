/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
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
