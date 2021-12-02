/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#ifndef KBLOCKSFIELD_H
#define KBLOCKSFIELD_H

#include "FieldInterface.h"

class KBlocksField : public FieldInterface
{
protected:
    bool **maBoard;
    int mHeight;
    int mWidth;

private:
    int mCurModifyID;
    int mLastModifyID;
    unsigned char *maEncodeData;

public:
    explicit KBlocksField(int width = 10, int height = 20);
    explicit KBlocksField(FieldInterface *p);
    ~KBlocksField() override;

public:
    bool getCell(int xPos, int yPos) override;
    void setCell(int xPos, int yPos, bool value);

    void copy(FieldInterface *p);
    void clear();

    bool checkFilledLine(int lineID);
    void removeFilledLine(int lineID);

    bool addPunishLine(int lineCount, int punishSeed);

    int  getModifyID();
    int  encodeData(unsigned char *data);
    void decodeData(unsigned char *data);

    int  getWidth() override;
    int  getHeight() override;

    bool equals(KBlocksField *rhs);

    int  getBlockHeight(int xPos);
    int  getFreeHeight(int xPos);
    void getSignature(int *data);
};

#endif

