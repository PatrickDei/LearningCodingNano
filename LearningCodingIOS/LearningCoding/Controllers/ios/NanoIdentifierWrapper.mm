//
//  NanoIdentifierWrapper.cpp
//  NanoGirl
//
//  Created by Dino Milačić on 14/03/16.
//
//
#include "NanoIdentifierWrapper.h"
#include "NanoIdentifier.h"
#include <string>

std::string NanoIdentifierWrapper::uuidForDevice()
{
    std::string udid = [[NanoIdentifier uuidForDevice] UTF8String];
    return udid;
}
