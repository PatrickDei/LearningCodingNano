//
//  PuzzlePiece.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 09/09/2020.
//

#include "PuzzlePiece.hpp"
#include "Puzzle.hpp"

int PuzzlePiece::getPositionIndex(){
    return positionIndex;
}

void PuzzlePiece::setPositionIndex(int index){
    positionIndex = index;
}


void PuzzlePiece::setAppropriatePosition(int index, float puzzleWidth, float mainImageScale, CCSize imageSize, float puzzleHeight){
    int i = index / NUM_OF_HORIZONTAL_PIECES;
    int j = index - i * NUM_OF_HORIZONTAL_PIECES;
    this->puzzlePieceImage->setPosition(ccp(i * puzzleWidth * mainImageScale, imageSize.height * mainImageScale - j * puzzleHeight * mainImageScale - puzzleHeight * mainImageScale));
}


PuzzlePiece& PuzzlePiece::operator = (PuzzlePiece& piece2){
    positionIndex = piece2.getPositionIndex();
    puzzlePieceImage = piece2.puzzlePieceImage;
    return *this;
}
