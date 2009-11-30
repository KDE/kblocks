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
    KBlocks_Animation_Max_Count,
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