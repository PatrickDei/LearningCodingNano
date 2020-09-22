//
//  Encoder.h
//  FiledCommander
//
//  Created by Saša Stanko on 15/07/2013.
//
//

#ifndef __FiledCommander__Encoder__
#define __FiledCommander__Encoder__

#include <map>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

namespace cocos2d
{
class CCDictionary;
class CCPoint;
class CCSize;
struct _ccColor3B;
struct _ccColor4B;
struct _ccColor4F;
}

class Serializable;
struct cJSON;

class Encoder
{
  public:
    ~Encoder();

    // basic types
    void encodeData(double data);
    void encodeData(int data) { encodeData((double)data); };
    void encodeData(bool &data);
    void encodeData(const char *data);

    // array & map
    template <typename T> void encodeArray(T first, T last)
    {
        beginArray();
        while(first != last) {
            add(*first);
            ++first;
        }
        endArray();
    }

    template <typename T> void encodeMap(T begin, T end)
    {
        beginMap();
        while(begin != end) {
            add((*begin).second, (*begin).first);
            ++begin;
        }
        endMap();
    }

    // vectors used for structures
    template <typename T> void encodeVector(T &v1, T &v2)
    {
        beginArray();
        add(v1);
        add(v2);
        endArray();
    }

    template <typename T> void encodeVector(T &v1, T &v2, T &v3)
    {
        beginArray();
        add(v1);
        add(v2);
        add(v3);
        endArray();
    }

    template <typename T> void encodeVector(T &v1, T &v2, T &v3, T &v4)
    {
        beginArray();
        add(v1);
        add(v2);
        add(v3);
        add(v4);
        endArray();
    }

    // STL types
    void encodeData(std::string &data) { encodeData(data.c_str()); }
    template <typename KeyType, typename ValueType> void encodeData(std::map<KeyType, ValueType> &aMap)
    {
        encodeMap(aMap.begin(), aMap.end());
    }
	
    template <typename ValueType> void encodeData(std::vector<ValueType> &data)
    {
        encodeArray(data.begin(), data.end());
    }
	
    template <typename ValueType> void encodeData(std::set<ValueType> &data) { encodeArray(data.begin(), data.end()); }
	
	template <typename T1, typename T2> void encodeData(std::pair<T1, T2> &aPair)
	{
		beginArray();
		add(aPair.first);
		add(aPair.second);
		endArray();
	}

    // serializable objects
    void encodeData(Serializable &data);
    void encodeData(Serializable *data);

    // cocos structures
    void encodeData(cocos2d::CCPoint &data);
    void encodeData(cocos2d::CCSize &data);
    void encodeData(cocos2d::_ccColor3B &data);
    void encodeData(cocos2d::_ccColor4B &data);
    void encodeData(cocos2d::_ccColor4F &data);

    // storing already serialized data from the stack
    void addEncoded();
    void addEncoded(const char *key);
    void addEncoded(const std::string &key) { addEncoded(key.c_str()); }
    template <typename T> void addEncoded(T key)
    {
        std::ostringstream oss;
        oss << key;
        addEncoded(oss.str());
    }

    // storing data
    template <typename T> void add(T data)
    {
        encodeData(data);
        addEncoded();
    }
    template <typename T, typename K> void add(T data, K key)
    {
        encodeData(data);
        addEncoded(key);
    }

    void beginMap();
    void beginArray();
    void endMap() {}
    void endArray() {}

    // save to file
    bool saveToFile(const char *filePath, bool formatted = true);
    char *createString(bool formatted = true);

    // encoding info
    void setInfoDict(cocos2d::CCDictionary *dict);
    cocos2d::CCDictionary *getInfoDict();

  private:
    std::stack<cJSON *> dataStack;
    cJSON *encodedData;

    cocos2d::CCDictionary *infoDict = nullptr;
};

#endif /* defined(__FiledCommander__Encoder__) */
