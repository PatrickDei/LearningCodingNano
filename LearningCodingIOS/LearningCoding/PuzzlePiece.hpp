//
//  PuzzlePiece.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 09/09/2020.
//

#ifndef PuzzlePiece_hpp
#define PuzzlePiece_hpp

#include <stdio.h>
#include "cocos2d.h"


#define NUM_OF_HORIZONTAL_PIECES 5
#define NUM_OF_VERTICAL_PIECES 5


USING_NS_CC;

class PuzzlePiece : public CCLayer{
    
private:
    int positionIndex;
    
public:
    CCSprite* puzzlePieceImage;
    
    PuzzlePiece(int index, CCSprite* image) : positionIndex(index), puzzlePieceImage(image){}
    
    int getPositionIndex();
    void setPositionIndex(int index);
    
    PuzzlePiece& operator = (PuzzlePiece& piece2);
    
    void setAppropriatePosition(int index, float puzzleWidth, float mainImageScale, CCSize imageSize, float puzzleHeight);
};

#endif /* PuzzlePiece_hpp */

