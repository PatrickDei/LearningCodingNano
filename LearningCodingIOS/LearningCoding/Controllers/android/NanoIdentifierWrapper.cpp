//
//  NanoIdentifierWrapper.cpp
//  NanoGirl
//
//  Created by Dino Milačić on 14/03/16.
//
//
#include "cocos2d.h"

#include "CCNative.h"
#include "NanoIdentifierWrapper.h"

//returns Android id
std::string  NanoIdentifierWrapper::uuidForDevice()
{
    return cocos2d::extension::CCNative::getOpenUDID();
}
