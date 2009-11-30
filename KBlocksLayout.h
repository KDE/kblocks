#ifndef KBLOCKSLAYOUT_H
#define KBLOCKSLAYOUT_H

#include "FieldInterface.h"
#include "PieceInterface.h"

#include <QPoint>

#define  PREPARE_AREA_WIDTH         5

class KBlocksLayout {
    public:
        KBlocksLayout();
        ~KBlocksLayout();
        
        void setGameField(FieldInterface * p);
        void setActivePiece(PieceInterface * p);
        void setNextPiece(PieceInterface * p);
        
        void initLayout();
        void freeLayout();
        
        void updateLayout(int lineCount = 0, int * lineList = 0);
        
        int getFieldColor(int posX, int posY);
        int getPrepareColor(int posX, int posY);
        
    private:
        FieldInterface* mpGameField;
        PieceInterface* mpActivePiece;
        PieceInterface* mpNextPiece;
        
        int** boardInfo;
        int** prepareInfo;
        
        QPoint** mpLastPiecePos;
};

#endif
