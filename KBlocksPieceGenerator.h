#ifndef KBLOCKSPIECEGENERATOR_H
#define KBLOCKSPIECEGENERATOR_H

class KBlocksPieceGenerator {
    public:
        KBlocksPieceGenerator(int size = 2048);
        ~KBlocksPieceGenerator();
        
        void genList(int seed);
        
        int getPiece();
        
    private:
        int  maxCapacity;
        int  pieceIndex;
        int* maPieceList;
};

#endif
