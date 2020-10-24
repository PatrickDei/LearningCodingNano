//
//  Puzzle.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 06/09/2020.
//

#include "Puzzle.hpp"
#include <algorithm>


bool Puzzle::init(){
    if (!CCLayer::init()){
        return false;
    }
    
    //adjustment for resolutions
    CCSprite* puzzleImage = CCSprite::create("HelloWorld.png");
    
    CCSize windowSize = CCDirector::sharedDirector()->getVisibleSize();
    imageSize = puzzleImage->getContentSize();
    
    //backbutton menu
    this->addChild(createBackButton(this, windowSize));
    
    
    //main scale for everything
    mainImageScale = windowSize.width / imageSize.width;
    
    //implementation for flexible number of pieces
    puzzleWidth = imageSize.width / NUM_OF_HORIZONTAL_PIECES;
    puzzleHeight = imageSize.height / NUM_OF_VERTICAL_PIECES;

    //set the sprites
    for(int i = 0; i < NUM_OF_HORIZONTAL_PIECES; i++){
        for(int j = 0; j < NUM_OF_VERTICAL_PIECES; j++){
            
            //setting up the puzzle pieces
            CCSprite* puzzlePiece = CCSprite::create("HelloWorld.png", CCRect(i * puzzleWidth, j * puzzleHeight, puzzleWidth, puzzleHeight));
            puzzlePiece->setAnchorPoint(CCPointZero);
            puzzlePiece->setScale(mainImageScale);
            this->addChild(puzzlePiece);
            PuzzlePiece* puzzleForVector = new PuzzlePiece(i * NUM_OF_HORIZONTAL_PIECES + j, puzzlePiece);
            puzzlePieces.push_back(puzzleForVector);
        }
    }
    //shuffle sprites from the beggining
    std::srand(std::time(0));
    std::random_shuffle(puzzlePieces.begin(), puzzlePieces.end());
    
    for(int i = 0; i < NUM_OF_HORIZONTAL_PIECES; i++){
        for(int j = 0; j < NUM_OF_VERTICAL_PIECES; j++){
            puzzlePieces[i * NUM_OF_HORIZONTAL_PIECES + j]->setAppropriatePosition(i * NUM_OF_HORIZONTAL_PIECES + j, puzzleWidth, mainImageScale, imageSize, puzzleHeight);
        }
    }
    
    //add the shuffle button
    CCMenuItemToggle* shuffleButton = CCMenuItemToggle::createWithTarget(this, menu_selector(Puzzle::shufflePieces), getButtonImage(), NULL);
    CCLabelTTF* shuffleButtonLabel = CCLabelTTF::create("Shuffle", "Arial", 24);
    shuffleButtonLabel->setPosition(ccp(shuffleButton->getContentSize().width / 2, shuffleButton->getContentSize().height / 2));
    shuffleButton->addChild(shuffleButtonLabel);
    shuffleButton->setPosition(ccp(windowSize.width / 2, windowSize.height / 4 * 3));
    shuffleButton->setScale(3);
    CCMenu* shuffleMenu = CCMenu::create(shuffleButton, NULL);
    shuffleMenu->setPosition(CCPointZero);
    this->addChild(shuffleMenu, 2);
    
    selectedPuzzle = false;
    
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);
    
    return true;
}

CCScene* Puzzle::scene(){
    
    CCScene* scene = CCScene::create();

    Puzzle* layer = Puzzle::create();
    
    scene->addChild(layer);
    
    return scene;
}

void Puzzle::shufflePieces(){
    
    std::srand(time(0));
    std::random_shuffle(puzzlePieces.begin(), puzzlePieces.end());
    
    //remove "you won" label if exists
    if(this->getChildByTag(1) != nullptr)
        this->removeChildByTag(1);
    
    for(int i = 0; i < NUM_OF_HORIZONTAL_PIECES; i++){
        for(int j = 0; j < NUM_OF_VERTICAL_PIECES; j++){
            puzzlePieces[i * NUM_OF_HORIZONTAL_PIECES + j]->setAppropriatePosition(i * NUM_OF_HORIZONTAL_PIECES + j, puzzleWidth, mainImageScale, imageSize, puzzleHeight);
        }
    }
}

bool Puzzle::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    CCPoint touch = pTouch->getLocation();
    if(tappedImage(touch)){
        
        selectedIndex = getPuzzlePieceIndex(touch);
        //@selectedPuzzle just says if its turn 1 or turn 2
        if(!selectedPuzzle){
            swapIndex = selectedIndex;
            selectedPuzzle = true;
        }
        else{
            swapPieces();
                
            if(checkForWin()){
                CCLabelTTF* winText = CCLabelTTF::create("You won!", "Arial", 64);
                winText->setPosition(ccp(CCDirector::sharedDirector()->getVisibleSize().width /2, imageSize.height * 3 / 2 * mainImageScale));
                this->addChild(winText, 1, 1);
            }
            selectedPuzzle = false;
        }
    }
    return true;
}


bool Puzzle::tappedImage(CCPoint touch){
    return (touch.x < imageSize.width * mainImageScale && touch.y < imageSize.height * mainImageScale) ? true : false;
}

int Puzzle::getPuzzlePieceIndex(CCPoint touch){
    int i = 0;
    //they are set up this was for now
    // 0 3 6
    // 1 4 7
    // 2 5 8
    
    //add from x
    i += NUM_OF_VERTICAL_PIECES - touch.y / (puzzleHeight * mainImageScale);
    
    //add from x
    i += (int)(touch.x / (puzzleWidth * mainImageScale)) * NUM_OF_VERTICAL_PIECES;
    
    return i;
}

void Puzzle::swapPieces(){
    
    float animationDuration = 1;
    CCPoint firstPiece = CCPoint(puzzlePieces[selectedIndex]->puzzlePieceImage->getPosition());
    CCPoint secondPiece = CCPoint(puzzlePieces[swapIndex]->puzzlePieceImage->getPosition());

    
    CCMoveBy* firstAnimation = CCMoveBy::create(animationDuration, secondPiece - firstPiece);
    CCMoveBy* secondAnimation = CCMoveBy::create(animationDuration, firstPiece - secondPiece);

    CCCallFunc* startOfAnimation = CCCallFunc::create(this, callfunc_selector(Puzzle::startOfAnimationFunc));
    CCCallFunc* endOfAnimation = CCCallFunc::create(this, callfunc_selector(Puzzle::endOfAnimationFunc));

    CCSequence* animationSequence = CCSequence::create(startOfAnimation, firstAnimation, endOfAnimation, NULL);
    
    //swapping
    puzzlePieces[selectedIndex]->puzzlePieceImage->runAction(animationSequence);
    puzzlePieces[swapIndex]->puzzlePieceImage->runAction(secondAnimation);
    
}

bool Puzzle::checkForWin(){
    for(int i = 0; i < NUM_OF_HORIZONTAL_PIECES; i++){
        for(int j = 0; j < NUM_OF_VERTICAL_PIECES; j++){
            if(puzzlePieces[i * NUM_OF_VERTICAL_PIECES + j]->getPositionIndex() != i * NUM_OF_VERTICAL_PIECES + j)
                return false;
        }
    }
    return true;
}

void Puzzle::startOfAnimationFunc(){
    setTouchEnabled(false);
    //overloaded operator =
    PuzzlePiece* temp = puzzlePieces[selectedIndex];
    puzzlePieces[selectedIndex] = puzzlePieces[swapIndex];
    puzzlePieces[swapIndex] = temp;
}

void Puzzle::endOfAnimationFunc(){
    setTouchEnabled(true);
}
