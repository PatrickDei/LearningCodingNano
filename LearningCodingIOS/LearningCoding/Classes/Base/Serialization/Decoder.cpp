//
//  Decoder.cpp
//  FiledCommander
//
//  Created by Saša Stanko on 16/07/2013.
//
//

#include "Decoder.h"

#include "NanoExtensions.h"
#include "ObjectFactory.h"
#include "cocos2d.h"
#include <sstream>
//#include "LevelObject.h"
#include "EncryptedNumbers.h"
#include "cJSON.h"
#include <fstream>

Decoder::~Decoder()
{
    while(!dataStack.empty()) {
        cJSON_Delete(dataStack.top());
        dataStack.pop();
    }

    CC_SAFE_RELEASE(infoDict);
}

#pragma mark - decoding basic types

void Decoder::decodeData(unsigned char &data)
{
    data = dataStack.top()->valueint;
    popData();
}

void Decoder::decodeData(int &data)
{
    data = dataStack.top()->valueint;
    popData();
}

void Decoder::decodeData(float &data)
{
    data = dataStack.top()->valuedouble;
    popData();
}

void Decoder::decodeData(double &data)
{
    data = dataStack.top()->valuedouble;
    popData();
}

void Decoder::decodeData(bool &data)
{
    if(dataStack.top()->type == cJSON_True) {
        data = true;
    } else if(dataStack.top()->type == cJSON_Number) {
        data = dataStack.top()->valueint;
    } else {
        data = false;
    }
    popData();
}

void Decoder::decodeData(std::string &data)
{
    data = dataStack.top()->valuestring;
    popData();
}

#pragma mark - decoding encrypted types

void Decoder::decodeData(EncriptedInt &data)
{
    data = dataStack.top()->valueint;
    popData();
}

void Decoder::decodeData(EncriptedFloat &data)
{
    data = dataStack.top()->valuedouble;
    popData();
}

void Decoder::decodeData(EncriptedDouble &data)
{
    data = dataStack.top()->valuedouble;
    popData();
}

#pragma mark - decoding cocos types

void Decoder::decodeData(cocos2d::CCPoint &data)
{
    get(data.x);
    get(data.y);
    popData();
}

void Decoder::decodeData(cocos2d::CCSize &data)
{
    get(data.width);
    get(data.height);
    popData();
}

void Decoder::decodeData(cocos2d::_ccColor3B &data)
{
    get(data.r);
    get(data.g);
    get(data.b);
    popData();
}

void Decoder::decodeData(cocos2d::_ccColor4B &data)
{
    get(data.r);
    get(data.g);
    get(data.b);
    get(data.a);
    popData();
}

void Decoder::decodeData(cocos2d::_ccColor4F &data)
{
    get(data.r);
    get(data.g);
    get(data.b);
    get(data.a);
    popData();
}

#pragma mark - decoding serializable objects

void Decoder::decodeData(Serializable &data)
{
    data.initWithDecoder(*this);
    popData();
}

void Decoder::decodeData(Serializable *&data)
{
    std::string className;
    get(className, "__class__");

    data = ObjectFactory::newInstance(className.c_str());
    if(data) {
        data->initWithDecoder(*this);
    }
    popData();
}

#pragma mark - getting keys

bool Decoder::getKey(std::string &key)
{
    key = dataStack.top()->string;
    return true;
}

#pragma mark - pushing encoded data to stack

bool Decoder::getEncoded()
{
    cJSON *data = iteratorStack.top();
    if(data) {
        iteratorStack.pop();
        iteratorStack.push(data->next);
        pushData(data);
        return true;
    }
    return false;
}

bool Decoder::getEncoded(const char *key)
{
    cJSON *data = dataStack.top()->child;
    while(data) {
        if(strcmp(data->string, key) == 0) {
            pushData(data);
            return true;
        }
        data = data->next;
    }
    return false;
}

bool Decoder::isEmpty() { return dataStack.empty(); }

#pragma mark - helper functions

void Decoder::pushData(cJSON *data)
{
    if(data) {
        dataStack.push(data);
        iteratorStack.push(data->child);
    }
}

void Decoder::popData()
{
    dataStack.pop();
    iteratorStack.pop();
}

#pragma mark - loading

bool Decoder::loadFromFile(const char *filePath)
{
    std::ifstream ifs(filePath);

    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    ifs.close();

    if(!content.empty()) {
        loadFromString(content.c_str());
    }

    return ifs.good();
}

void Decoder::loadFromString(const char *encodedData)
{
    // std::cout<<"Read file:\n"<<encodedData<<std::endl;
    if(cJSON *data = cJSON_Parse(encodedData)) {
        pushData(data);
    }
}

#pragma mark - decoding info

void Decoder::setInfoDict(cocos2d::CCDictionary *dict) { assignRetainObject(infoDict, dict); }

cocos2d::CCDictionary *Decoder::getInfoDict()
{
    if(infoDict == nullptr) {
        infoDict = cocos2d::CCDictionary::create();
        infoDict->retain();
    }
    return infoDict;
}
