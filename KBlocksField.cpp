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

KBlocksField::KBlocksField(int w, int h)
{
    width = w;
    height = h;
    
    board = new bool*[height]();
    for(int i = 0; i < height; i++)
    {
        board[i] = new bool[width];
        for(int j = 0; j < width; j++)
        {
            board[i][j] = false;
        }
    }
}

KBlocksField::~KBlocksField()
{
    for(int i = 0; i < height; i++)
    {
        delete board[i];
    }
    delete [] board;
}

bool KBlocksField::getCell(int xPos, int yPos)
{
    if ( (xPos < 0) || (xPos >= width) || (yPos >= height) )
    {
        return true;
    }
    if (yPos < 0)
    {
        return false; // Allow blocks to be placed above top
    }
    
    return board[yPos][xPos];
}

void KBlocksField::setCell(int xPos, int yPos, bool value)
{
    if ( ((xPos < 0) || (xPos >= width))
      || ((yPos < 0) || (yPos >= height)) )
    {
        return;
    }
    board[yPos][xPos] = value;
}

void KBlocksField::clearAll()
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            board[i][j] = false;
        }
    }
}

bool KBlocksField::checkFilledLine(int lineID)
{
    for(int i = 0; i < width; i++)
    {
        if (board[lineID][i] == false)
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
        for(int j = 0; j < width; j++)
        {
            board[i][j] = board[i-1][j];
        }
    }
}

bool KBlocksField::addPunishLine(int emptyID)
{
    bool result = true;
    
    if (emptyID == -1)
    {
        for(int i = 0; i < width; i++)
        {
            if (board[height-1][i])
            {
                emptyID = i;
                break;
            }
        }
    }
    
    for(int i = 0; i < width; i++)
    {
        if (board[0][i])
        {
            result = false;
            break;
        }
    }
    
    for(int i = 0; i < height - 1; i++)
    {
        for(int j = 0; j < width; j++)
        {
            board[i][j] = board[i+1][j];
        }
    }
    
    for(int i = 0; i < width; i++)
    {
        board[height-1][i] = true;
    }
    board[height-1][emptyID] = false;
    
    return result;
}

int KBlocksField::getWidth()
{
    return width;
}

int KBlocksField::getHeight()
{
    return height;
}
