//
//  Decoder.h
//  FiledCommander
//
//  Created by Saša Stanko on 16/07/2013.
//
//

#ifndef __FiledCommander__Decoder__
#define __FiledCommander__Decoder__

#include <deque>
#include <map>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

struct cJSON;

class Serializable;
class EncriptedInt;
class EncriptedFloat;
class EncriptedDouble;

namespace cocos2d
{
class CCDictionary;
class CCPoint;
class CCSize;
class CCObject;
struct _ccColor3B;
struct _ccColor4B;
struct _ccColor4F;
}

class Decoder
{
  public:
    ~Decoder();

#pragma mark - decoding basic types
    void decodeData(unsigned char &t);
    void decodeData(int &t);
    void decodeData(float &t);
    void decodeData(double &t);
    void decodeData(bool &t);
    void decodeData(std::string &t);

#pragma mark - decoding encrypted types
    void decodeData(EncriptedInt &t);
    void decodeData(EncriptedFloat &t);
    void decodeData(EncriptedDouble &t);

#pragma mark - decoding cocos types
    void decodeData(cocos2d::CCPoint &data);
    void decodeData(cocos2d::CCSize &data);
    void decodeData(cocos2d::_ccColor3B &data);
    void decodeData(cocos2d::_ccColor4B &data);
    void decodeData(cocos2d::_ccColor4F &data);

#pragma mark - decoding serializable objects
    // data object is NOT autoreleased
    template <typename T> void decodeData(T *&data)
    {
        Serializable *object = nullptr;
        decodeData(object);
        data = static_cast<T *>(object);
    }

    void decodeData(Serializable *&data);
    void decodeData(Serializable &data);

#pragma mark - decoding std containers
    template <typename T> void decodeData(std::vector<T> &data)
    {
        data.clear();
        while(getEncoded()) {
            T element;
            decodeData(element);
            data.push_back(element);
        }
        popData();
    }

    template <typename T> void decodeData(std::set<T> &data)
    {
        data.clear();
        while(getEncoded()) {
            T element;
            decodeData(element);
            data.insert(element);
        }
        popData();
    }

    template <typename KeyType, typename ValueType> void decodeData(std::map<KeyType, ValueType> &data)
    {
        data.clear();
        while(getEncoded()) {
            ValueType element;
            KeyType key;
            getKey(key);
            decodeData(element);
            data[key] = element;
        }
        popData();
    }
	
	template <typename T1, typename T2> void decodeData(std::pair<T1, T2> &data)
	{
		get(data.first);
		get(data.second);
		popData();
	}

#pragma mark - getting keys
    bool getKey(std::string &key);
    template <typename T> bool getKey(T &key)
    {
        std::string keyString;
        if(getKey(keyString)) {
            std::istringstream iss(keyString);
            iss >> key;
            return true;
        }
        return false;
    }

#pragma mark - pushing encoded data to stack
    bool getEncoded();
    bool getEncoded(const char *key);
    bool getEncoded(const std::string &key) { return getEncoded(key.c_str()); }
    template <typename T> bool getEncoded(T key)
    {
        std::ostringstream oss;
        oss << key;
        return getEncoded(oss.str());
    }

#pragma mark - getting decoded data
    template <typename T> bool get(T &data)
    {
        if(getEncoded()) {
            decodeData(data);
            return true;
        }
        return false;
    }
    template <typename T, typename K> bool get(T &data, K key)
    {
        if(getEncoded(key)) {
            decodeData(data);
            return true;
        }
        return false;
    }

    // functions for getting CCObjects
    // release existing object, and retain the new one
    template <typename T> bool get(T *&data)
    {
        Serializable *obj = nullptr;
        if(getEncoded()) {
            decodeData(obj);
            if(obj) {
                if(data)
                    data->release();
                data = static_cast<T *>(obj);
                return true;
            }
        }
        return false;
    }

    template <typename T, typename K> bool get(T *&data, K key)
    {
        Serializable *obj = nullptr;
        if(getEncoded(key)) {
            decodeData(obj);
            if(obj) {
                if(data)
                    data->release();
                data = static_cast<T *>(obj);
                return true;
            }
        }
        return false;
    }

    // functions for getting CCObjects
    // returns autoreleased object
    template <typename T = cocos2d::CCObject> T *getObject()
    {
        Serializable *obj = nullptr;
        if(getEncoded()) {
            decodeData(obj);
        }
        T *data = static_cast<T *>(obj);
        data->autorelease();
        return data;
    }

    template <typename T = cocos2d::CCObject, typename K> T *getObjectForKey(K key)
    {
        Serializable *obj = nullptr;
        if(getEncoded(key)) {
            decodeData(obj);
        }
        T *data = static_cast<T *>(obj);
        data->autorelease();
        return data;
    }

#pragma mark - helper functions
    void pushData(cJSON *data);
    void popData();

#pragma mark - loading encoded data
    bool loadFromFile(const char *filePath);
    void loadFromString(const char *encodedData);
    bool isEmpty();

    //	void getKeys (std::vector<std::string>& keys);

    // encoding info
    void setInfoDict(cocos2d::CCDictionary *dict);
    cocos2d::CCDictionary *getInfoDict();

  private:
    std::stack<cJSON *> dataStack;
    std::stack<cJSON *> iteratorStack;

    cocos2d::CCDictionary *infoDict = nullptr;
};

#endif /* defined(__FiledCommander__Decoder__) */
