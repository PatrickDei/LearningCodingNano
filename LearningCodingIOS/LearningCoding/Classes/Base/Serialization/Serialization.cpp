//
//  Serialization.cpp
//  FiledCommander
//
//  Created by Saša Stanko on 11/07/2013.
//
//

#include "Serialization.h"

std::string Serializable::encodeType() { return typeid(*this).name(); }
