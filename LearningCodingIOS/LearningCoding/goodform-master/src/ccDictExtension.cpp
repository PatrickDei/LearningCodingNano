//
//  ccDictExtension.cpp
//  NanoGirl
//
//  Created by Marino Loncar on 07/02/17.
//
//

#include "ccDictExtension.hpp"
#include <string>
#include "CCNumber.h"

namespace goodform
{
    cocos2d::CCObject* ccDictExtension::deserialize(const variant& v)
    {
        if (v.is<std::nullptr_t>())
        {
            return cocos2d::CCDictionary::create();
        }
        else if (v.is<bool>())
        {
            return cocos2d::CCBool::create(v.get<bool>());
        }
        else if (v.type() == variant_type::signed_integer)
        {
            return cocos2d::CCNumber::create((int)v.get<std::int64_t>());
        }
        else if (v.type() == variant_type::unsigned_integer)
        {
            return cocos2d::CCNumber::create((int)(v.get<std::uint64_t>()));
        }
        else if (v.type() == variant_type::floating_point)
        {
            return cocos2d::CCNumber::create((double)(v.get<double>()));
        }
        else if (v.is<std::string>())
        {
            return cocos2d::CCString::create(v.get<std::string>());
        }
        else if (v.is<goodform::array>())
        {
            return convertArray(v);
        }
        else if (v.is<goodform::object>())
        {
            return convertObject(v);
        }
        else if (v.is<goodform::binary>())
        {
            std::string binaryString(v.get<goodform::binary>().begin(), v.get<goodform::binary>().end());
            return cocos2d::CCString::create(binaryString);
        }
        
        return nullptr;

    }
    
    
    
    cocos2d::CCArray* ccDictExtension::convertArray(const variant& v)
    {
        cocos2d::CCArray * newArray = cocos2d::CCArray::create();
        for(auto var : v.get<goodform::array>())
        {
            auto newObject = deserialize(var);
            newArray->addObject(newObject);
        }
        return newArray;
    }
    
    cocos2d::CCDictionary* ccDictExtension::convertObject(const variant& v)
    {
        cocos2d::CCDictionary* newDict = cocos2d::CCDictionary::create();
        
        for(auto elem : v.get<goodform::object>())
        {
            auto newObject = deserialize(elem.second);
            newDict->setObject(newObject, elem.first);
        }
        
        return newDict;
    }
}
