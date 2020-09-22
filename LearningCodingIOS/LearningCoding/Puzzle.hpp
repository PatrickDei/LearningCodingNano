//
//  Puzzle.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 06/09/2020.
//

#ifndef Puzzle_hpp
#define Puzzle_hpp

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include "PuzzlePiece.hpp"
#include "HelloWorldScene.h"

USING_NS_CC;

class Puzzle : public cocos2d::CCLayer{
public:
    
    virtual bool init() override;

    void callScene(){
        CCDirector::sharedDirector()->replaceScene(Puzzle::scene());
    }
    static CCScene* scene();
    
    void returnToMainMenu(){
        for(auto p : puzzlePieces)
            delete p;
        CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
    }
    
    CREATE_FUNC(Puzzle);

private:
    
    bool selectedPuzzle;
    
    float puzzleWidth;
    
    float puzzleHeight;
    
    float mainImageScale;
    
    std::vector<PuzzlePiece*> puzzlePieces;
    
    CCSize imageSize;
    
    CCSize windowSize;
    
    int swapIndex;
    
    int selectedIndex;
    
    
    
    float scaleMainImage(CCSize windowSize, CCSize imageSize);
    
    void shufflePieces();

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override;
    
    bool tappedImage(CCPoint touch);
    
    int getPuzzlePieceIndex(CCPoint touch);
    
    void swapPieces();
    
    bool checkForWin();
    
    void startOfAnimationFunc();
    
    void endOfAnimationFunc();
};

#endif /* Puzzle_hpp */
