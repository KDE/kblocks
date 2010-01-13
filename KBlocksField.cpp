/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksField.h"

KBlocksField::KBlocksField(int width, int height)
{
    mWidth = width;
    mHeight = height;
    
    mCurModifyID = 0;
    mLastModifyID = -1;
    
    maEncodeData = new unsigned char[width * height / 8 + 1];
    
    maBoard = new bool*[mHeight]();
    for(int i = 0; i < mHeight; i++)
    {
        maBoard[i] = new bool[mWidth];
        for(int j = 0; j < mWidth; j++)
        {
            maBoard[i][j] = false;
        }
    }
}

KBlocksField::KBlocksField(FieldInterface * p)
{
    mWidth = p->getWidth();
    mHeight = p->getHeight();
    
    mCurModifyID = 0;
    mLastModifyID = -1;
    
    maEncodeData = new unsigned char[mWidth * mHeight / 8 + 1]();
    
    maBoard = new bool*[mHeight]();
    for(int i = 0; i < mHeight; i++)
    {
        maBoard[i] = new bool[mWidth];
        for(int j = 0; j < mWidth; j++)
        {
            maBoard[i][j] = p->getCell(j, i);
        }
    }
}

KBlocksField::~KBlocksField()
{
    for(int i = 0; i < mHeight; i++)
    {
        delete maBoard[i];
    }
    delete [] maBoard;
    delete [] maEncodeData;
}

bool KBlocksField::getCell(int xPos, int yPos)
{
    if ( (xPos < 0) || (xPos >= mWidth) || (yPos >= mHeight) )
    {
        return true;
    }
    if (yPos < 0)
    {
        return false; // Allow blocks to be placed above top
    }
    
    return maBoard[yPos][xPos];
}

void KBlocksField::setCell(int xPos, int yPos, bool value)
{
    if ( ((xPos < 0) || (xPos >= mWidth))
      || ((yPos < 0) || (yPos >= mHeight)) )
    {
        return;
    }
    maBoard[yPos][xPos] = value;
    mCurModifyID += 1;
}

void KBlocksField::copy(FieldInterface * p)
{
    for(int i = 0; i < mHeight; i++)
    {
        for(int j = 0; j < mWidth; j++)
        {
            maBoard[i][j] = p->getCell(j, i);
        }
    }
}

void KBlocksField::clear()
{
    for(int i = 0; i < mHeight; i++)
    {
        for(int j = 0; j < mWidth; j++)
        {
            maBoard[i][j] = false;
        }
    }
    mCurModifyID = 0;
}

bool KBlocksField::checkFilledLine(int lineID)
{
    for(int i = 0; i < mWidth; i++)
    {
        if (maBoard[lineID][i] == false)
        {
            return false;
        }
    }
    return true;
}

void KBlocksField::removeFilledLine(int lineID)
{
    for(int i = lineID; i > 0; i--)
    {
        for(int j = 0; j < mWidth; j++)
        {
            maBoard[i][j] = maBoard[i-1][j];
        }
    }
    mCurModifyID += 1;
}

bool KBlocksField::addPunishLine(int lineCount, int punishSeed)
{
    bool result = true;
    
    for(int i = 0; i < mWidth; i++)
    {
        if (maBoard[0][i])
        {
            result = false;
            break;
        }
    }
    
    for(int i = 0; i < mHeight - 1; i++)
    {
        for(int j = 0; j < mWidth; j++)
        {
            maBoard[i][j] = maBoard[i+1][j];
        }
    }
    
    for(int i = 0; i < mWidth; i++)
    {
        maBoard[mHeight-1][i] = true;
    }
    
    for(int i = 0; i < lineCount; i++)
    {
        maBoard[mHeight-1][punishSeed] = false;
        punishSeed = (punishSeed + lineCount) % mWidth;
    }
    
    mCurModifyID += 1;
    
    return result;
}

int KBlocksField::getModifyID()
{
    return mCurModifyID;
}

int KBlocksField::encodeData(unsigned char * data)
{
    int byteCounter = 0;
    int loopCounter = 0;
    
    if (mLastModifyID != mCurModifyID)
    {
        mLastModifyID = mCurModifyID;
        for(int i = 0; i < mHeight; i++)
        {
            for(int j = 0; j < mWidth; j++)
            {
                byteCounter = loopCounter / 8;
                if (maBoard[i][j])
                {
                    maEncodeData[byteCounter] |= (1 << (loopCounter % 8));
                }
                else
                {
                    maEncodeData[byteCounter] &= ~(1 << (loopCounter % 8));
                }
                loopCounter++;
            }
        }
    }
    
    byteCounter = mWidth * mHeight / 8;
    for(int i = 0; i < byteCounter; i++)
    {
        data[i] = maEncodeData[i];
    }
    
    return byteCounter;
}

void KBlocksField::decodeData(unsigned char * data)
{
    int byteCounter = 0;
    int loopCounter = 0;
    
    for(int i = 0; i < mHeight; i++)
    {
        for(int j = 0; j < mWidth; j++)
        {
            byteCounter = loopCounter / 8;
            if (((data[byteCounter] >> (loopCounter % 8)) & 0x01) == 0x01)
            {
                maBoard[i][j] = true;
            }
            else
            {
                maBoard[i][j] = false;
            }
            loopCounter++;
        }
    }
    
    for(int i = 0; i < byteCounter; i++)
    {
        maEncodeData[i] = data[i];
    }
    
    mCurModifyID = 0;
    mLastModifyID = 0;
}

int KBlocksField::getWidth()
{
    return mWidth;
}

int KBlocksField::getHeight()
{
    return mHeight;
}
