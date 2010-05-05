/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2009 Mauricio Piacentini <mauricio@tabuleiro.com>       *
*                      Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef KBLOCKSANIMATOR_H
#define KBLOCKSANIMATOR_H

#include <QTimeLine>

#include "KBlocksAnimFade.h"
#include "KBlocksAnimDrop.h"

#include "KBlocksSvgItem.h"

enum KBlocks_Animation_Type {
    KBlocks_Animation_None = 0,
    KBlocks_Animation_Fade_In,
    KBlocks_Animation_Fade_Out,
    KBlocks_Animation_Drop,
    KBlocks_Animation_Max_Count
};

class KBlocksAnimator : public QObject
{
    Q_OBJECT
    
    public:
        KBlocksAnimator();
        ~KBlocksAnimator();
        
        bool createFadeAnim(const QList<KBlocksSvgItem*> & items, int duration, QTimeLine::Direction direction);
        bool deleteFadeAnim();
        KBlocksAnimFade* getFadeAnim();
        
        bool createDropAnim(const QList<KBlocksSvgItem*> & items, int duration, QTimeLine::Direction direction);
        bool deleteDropAnim();
        KBlocksAnimDrop* getDropAnim();
        
    private slots:
        void endFadeInAnim();
        void endFadeOutAnim();
        void endDropAnim();
        
    signals:
        void animFinished(int animType);
        
    protected:
        KBlocksAnimFade* mpAnimFade;
        KBlocksAnimDrop* mpAnimDrop;
};

#endif
