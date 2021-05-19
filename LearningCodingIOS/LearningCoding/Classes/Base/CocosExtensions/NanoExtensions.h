//
//  NanoExtensions.h
//  NanoGirl
//
//  Created by Luka Piljek on 7/15/13.
//
//

#ifndef __NanoGirl__NanoExtensions__
#define __NanoGirl__NanoExtensions__

#include "cocos2d.h"
#include <zlib.h>
//#include "SoundManager.h"

NS_CC_BEGIN

// time in seconds
#define MINUTE 60
#define HALF_HOUR 1800
#define HOUR 3600
#define DAY 86400

#define WEEKS_PER_MONTH 4
#define WEEKS_PER_YEAR 52

// relative positions
#define leftDown ccp(0.0f, 0.0f)
#define leftMid ccp(0.0f, 0.5f)
#define leftUp ccp(0.0f, 1.0f)
#define midDown ccp(0.5f, 0.0f)
#define midMid ccp(0.5f, 0.5f)
#define midUp ccp(0.5f, 1.0f)
#define rightDown ccp(1.0f, 0.0f)
#define rightMid ccp(1.0f, 0.5f)
#define rightUp ccp(1.0f, 1.0f)

#define MAX_STRING_LEN (1024 * 100)
#define MAX_NAME_LEN 64

// important singletons
#define CCSharedDirector CCDirector::sharedDirector()
#define CCSharedFileUtils CCFileUtils::sharedFileUtils()
#define CCSharedNotificationCenter CCNotificationCenter::sharedNotificationCenter()
#define CCSharedUserDefaults CCUserDefault::sharedUserDefault()

#define CCARRAY_FOREACH_TYPE(__array_expression__, __obj_type__, __obj_name__)                                         \
    if (cocos2d::CCArray *__array__ = (__array_expression__))                                                          \
        if ((__array__)->data->num > 0)                                                                                \
            for (cocos2d::CCObject **__arr__ = (__array__)->data->arr,                                                 \
                                   **__end__ = (__array__)->data->arr + (__array__)->data->num - 1;                    \
                 __arr__ <= __end__ && (*__arr__ != NULL /* || true*/); __arr__++)                                     \
                if (__obj_type__ *__obj_name__ = static_cast<__obj_type__ *>(*__arr__))

#define CCARRAY_FOREACH_TYPE_SAFE(__array_expression__, __obj_type__, __obj_name__)                                    \
    if (cocos2d::CCArray *__array__ = (__array_expression__))                                                          \
        if ((__array__)->data->num > 0)                                                                                \
            for (cocos2d::CCObject **__arr__ = (__array__)->data->arr,                                                 \
                                   **__end__ = (__array__)->data->arr + (__array__)->data->num - 1;                    \
                 __arr__ <= __end__ && (*__arr__ != NULL /* || true*/); __arr__++)                                     \
                if (__obj_type__ *__obj_name__ = dynamic_cast<__obj_type__ *>(*__arr__))

#define CCARRAY_FOREACH_TYPE_REVERSE(__array_expression__, __obj_type__, __obj_name__)                                 \
    if (cocos2d::CCArray *__array__ = (__array_expression__))                                                          \
        if ((__array__)->data->num > 0)                                                                                \
            for (cocos2d::CCObject **__arr__ = (__array__)->data->arr + (__array__)->data->num - 1,                    \
                                   **__end__ = (__array__)->data->arr;                                                 \
                 __arr__ >= __end__ && (*__arr__ != NULL /* || true*/); __arr__--)                                     \
                if (__obj_type__ *__obj_name__ = static_cast<__obj_type__ *>(*__arr__))

#define CCARRAY_FOREACH_TYPE_SAFE_REVERSE(__array_expression__, __obj_type__, __obj_name__)                            \
    if (cocos2d::CCArray *__array__ = (__array_expression__))                                                          \
        if ((__array__)->data->num > 0)                                                                                \
            for (cocos2d::CCObject **__arr__ = (__array__)->data->arr + (__array__)->data->num - 1,                    \
                                   **__end__ = (__array__)->data->arr;                                                 \
                 __arr__ >= __end__ && (*__arr__ != NULL /* || true*/); __arr__--)                                     \
                if (__obj_type__ *__obj_name__ = dynamic_cast<__obj_type__ *>(*__arr__))

#define CC_ASSIGN_RETAIN(var, value)                                                                                   \
    do {                                                                                                               \
        if (var) {                                                                                                     \
            (var)->release();                                                                                          \
        }                                                                                                              \
        (var) = (value);                                                                                               \
        if (var) {                                                                                                     \
            (var)->retain();                                                                                           \
        }                                                                                                              \
    } while (0)

class CommaNumpunct : public std::numpunct<char>
{
  protected:
    virtual char do_thousands_sep() const { return ','; }
    virtual std::string do_grouping() const { return "\03"; }
};

bool operator==(const ccColor3B &first, const ccColor3B &second);
bool operator!=(const ccColor3B &first, const ccColor3B &second);

NS_CC_END

template <typename T1, typename T2> void assignRetainObject(T1 var, T2 value)
{
    if (var)
        var->release();
    var = static_cast<T1>(value);
    if (var)
        var->retain();
}

template <typename T> T getRetainedObject(T obj)
{
    if (obj)
        obj->retain();
    return obj;
}

template <typename T> T getRetainedObject(cocos2d::CCObject *obj)
{
    if (obj)
        obj->retain();
    return static_cast<T>(obj);
}

template <typename T, typename... Args> T *createObjectFunction(Args... args)
{
    T *pRet = new T();
    if (pRet && pRet->init(args...)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

template <class T> void removeAllChildrenByType(cocos2d::CCNode *node)
{
    cocos2d::CCArray *toDelete = cocos2d::CCArray::create();
    CCARRAY_FOREACH_TYPE_SAFE(node->getChildren(), T, child) { toDelete->addObject(child); }
    CCARRAY_FOREACH_TYPE(toDelete, cocos2d::CCNode, object) { object->removeFromParent(); }
}

#pragma mark - shaders

cocos2d::CCGLProgram *getBlurShader();

#pragma mark - helper functions

//void buyIfPossible(float priceTickets, float priceGems, std::function<void()> yesFunction = nullptr,
//                   std::function<void()> noFunction = nullptr, cocos2d::CCNode *buyButton = nullptr,
//                   const char *soundEffect = nullptr, bool showOneTimeOffer = false);
//std::string getNereestInAppPurchase(int gemsAmount);
//std::string getInGameInAppPurchase(int gemsAmount, bool showOneTimeOffer);
int compareVersions(const std::string &firstVersion, const std::string &secondVersion);

std::string stringFromInt(int);
std::string stringFromFloat(float);
std::string prettyStringFromInt(int, int);
std::string clockStringFromSeconds(double);
std::string clockStringFromSecondsShort(double);
std::string timeStringFromSeconds(double, bool hideLoweTime = true);
std::string timeStringFromTimestamp(int);
std::string timeStringFromTimestampUTC(int);
std::string dateStringFromTimestamp(int);
int minutesInDayFromTimestamp(int);
bool checkIfSameDate(int, int);

bool stringCaseInsensitiveIsEqual(std::string strFirst, std::string strSecond);
bool stringReplace(std::string &str, const std::string &from, const std::string &to);

void printCCDictionary(std::string key, cocos2d::CCDictionary *dict);

void storeStringToFile(std::string str, std::string full_path);
void readStringFromFile(std::string &str, std::string full_path);

typedef bool (*pLogComparison)(cocos2d::CCDictionary *, cocos2d::CCDictionary *);
cocos2d::CCArray *mergeSortedArrays(cocos2d::CCArray *a1, cocos2d::CCArray *a2, pLogComparison comparison);

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

bool isCountryUS(std::string countryCode);
bool isCountryEU(std::string countryCode);

#endif /* defined(__NanoGirl__NanoExtensions__) */
