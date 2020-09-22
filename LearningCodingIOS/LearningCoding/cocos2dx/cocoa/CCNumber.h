//
//  CCNumber.h
//  Hollywood
//
//  Created by Matko Juribasic on 12/11/14.
//
//

#ifndef Hollywood_CCNumber_h
#define Hollywood_CCNumber_h

#include "CCObject.h"
namespace cocos2d
{
    
    class CCNumber : public cocos2d::CCObject {
    private:
        int m_nValue;
        double m_dValue;
    public:
        CCNumber(int v)
        : m_nValue(v),m_dValue(v) {}
        
        CCNumber(double v)
        : m_nValue(v),m_dValue(v) {}
        
        virtual CCObject* copyWithZone(CCZone* pZone)
        {
            CCNumber* pStr = new CCNumber(m_nValue);
            return pStr;
        }

        
        static CCNumber * create(int v)
        {
            CCNumber* pRet = new CCNumber(v);
            pRet->autorelease();
            return pRet;
        }
        static CCNumber* create(double v)
        {
            CCNumber* pRet = new CCNumber(v);
            pRet->autorelease();
            return pRet;
        }
        int getIntValue() const {return m_nValue;}
        double getDoubleValue() const {return m_dValue;}
        void setValue(int v)  { m_nValue=v; m_dValue=v;}
        void setValue(double v)  { m_nValue=v; m_dValue=v;}
    };
}

#endif
