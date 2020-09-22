//
//  ccDictExtension.h
//  NanoGirl
//
//  Created by Marino Loncar on 07/02/17.
//
//

#ifndef ccDictExtension_hpp
#define ccDictExtension_hpp

#import "cocos2d.h"
#import "variant.hpp"


//######################################################################//
namespace goodform
{
    //**********************************************************************//
    class ccDictExtension
    {
    private:
        ccDictExtension() = delete;
        
    public:
        static cocos2d::CCObject* deserialize(const variant& v);
        
        static cocos2d::CCArray* convertArray(const variant& v);
        
        static cocos2d::CCDictionary* convertObject(const variant& v);
    };
}


#endif /* ccDictExtension_h */
