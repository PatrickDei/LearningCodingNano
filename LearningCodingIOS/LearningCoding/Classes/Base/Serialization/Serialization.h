//
//  Serialization.h
//  FiledCommander
//
//  Created by Saša Stanko on 11/07/2013.
//
//

#ifndef __FiledCommander__Serialization__
#define __FiledCommander__Serialization__

#include "Decoder.h"
#include "Encoder.h"
#include <string>

class Serializable
{
  public:
    virtual std::string encodeType();

    virtual void encode(Encoder &encoder) {}
    virtual void initWithDecoder(Decoder &decoder) {}

    virtual ~Serializable() {}
};

#endif /* defined(__FiledCommander__Serialization__) */
