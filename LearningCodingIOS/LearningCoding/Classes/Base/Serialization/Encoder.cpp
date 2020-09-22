//
//  Encoder.cpp
//  FiledCommander
//
//  Created by Saša Stanko on 15/07/2013.
//
//
#include "Encoder.h"
#include "NanoExtensions.h"
#include "cJSON.h"
#include "cocos2d.h"

#include <fstream>

Encoder::~Encoder()
{
    while(!dataStack.empty()) {
        cJSON_Delete(dataStack.top());
        dataStack.pop();
    }

    CC_SAFE_RELEASE(infoDict);
}

#pragma mark - encoding serializable objects

void Encoder::encodeData(Serializable &data)
{
    beginMap();
    add(data.encodeType(), "__class__");
    data.encode(*this);
    endMap();
}

void Encoder::encodeData(Serializable *data)
{
    if(data) {
        beginMap();
        add(data->encodeType(), "__class__");
        data->encode(*this);
        endMap();
    } else {
        dataStack.push(nullptr);
    }
}

#pragma mark - encoding basic types

void Encoder::encodeData(double data) { dataStack.push(cJSON_CreateNumber(data)); }

void Encoder::encodeData(bool &data) { dataStack.push(cJSON_CreateBool(data)); }

void Encoder::encodeData(const char *data) { dataStack.push(cJSON_CreateString(data)); }

#pragma mark - encoding cocos types

void Encoder::encodeData(cocos2d::CCPoint &data) { encodeVector(data.x, data.y); }

void Encoder::encodeData(cocos2d::CCSize &data) { encodeVector(data.width, data.height); }

void Encoder::encodeData(cocos2d::_ccColor3B &data) { encodeVector(data.r, data.g, data.b); }

void Encoder::encodeData(cocos2d::_ccColor4B &data) { encodeVector(data.r, data.g, data.b, data.a); }

void Encoder::encodeData(cocos2d::_ccColor4F &data) { encodeVector(data.r, data.g, data.b, data.a); }

#pragma mark - storing serialized data from stack

void Encoder::addEncoded()
{
    cJSON *encodedData = dataStack.top();
    dataStack.pop();
    cJSON_AddItemToArray(dataStack.top(), encodedData);
}

void Encoder::addEncoded(const char *key)
{
    cJSON *encodedData = dataStack.top();
    dataStack.pop();
    cJSON_AddItemToObject(dataStack.top(), key, encodedData);
}

#pragma mark - helper methods

void Encoder::beginMap() { dataStack.push(cJSON_CreateObject()); }

void Encoder::beginArray() { dataStack.push(cJSON_CreateArray()); }

#pragma mark - save

char *Encoder::createString(bool formatted)
{
    if(dataStack.empty()) {
        return nullptr;
    }
    if(formatted) {
        return cJSON_Print(dataStack.top());
    } else {
        return cJSON_PrintUnformatted(dataStack.top());
    }
}

bool Encoder::saveToFile(const char *filePath, bool formatted)
{
    char *encodedString = createString(formatted);
    if(encodedString == nullptr)
        return false;

    std::ofstream ofs(filePath);
    ofs << encodedString;
    ofs.close();

    free(encodedString);

    return ofs.good();
}

#pragma mark - encoding info

void Encoder::setInfoDict(cocos2d::CCDictionary *dict) { assignRetainObject(infoDict, dict); }

cocos2d::CCDictionary *Encoder::getInfoDict()
{
    if(infoDict == nullptr) {
        infoDict = cocos2d::CCDictionary::create();
        infoDict->retain();
    }
    return infoDict;
}
