//
//  ObjectFactory.h
//  NanoGirl
//
//  Created by Luka Piljek on 7/12/13.
//
//

#ifndef __NanoGirl__ObjectFactory__
#define __NanoGirl__ObjectFactory__

#include <map>
#include <string>

class Serializable;

template <typename T> Serializable *createT() { return new T(); }

struct ObjectFactory {
    typedef std::map<std::string, Serializable *(*)()> factory_map_type;

    static Serializable *newInstance(std::string const &s)
    {
        factory_map_type::iterator it = getFactoryMap()->find(s);
        if(it == getFactoryMap()->end())
            return nullptr;
        return it->second();
    }

  protected:
    static factory_map_type *getFactoryMap()
    {
        // never delete'ed. (exist until program termination)
        // because we can't guarantee correct destruction order
        if(!factoryMap) {
            factoryMap = new factory_map_type;
        }
        return factoryMap;
    }

  private:
    static factory_map_type *factoryMap;
};

template <typename T> struct FactoryRegister : ObjectFactory {
    FactoryRegister(std::string const &s) { getFactoryMap()->insert(std::make_pair(s, &createT<T>)); }
};

#define REGISTER_DEC_TYPE(NAME) static FactoryRegister<NAME> factoryReg

#define REGISTER_DEF_TYPE(NAME) FactoryRegister<NAME> NAME::factoryReg(#NAME)

#define CODING_DECLARE(NAME)                                                                                           \
    static FactoryRegister<NAME> factoryReg;                                                                           \
    virtual std::string encodeType();                                                                                  \
    virtual void encode(Encoder &encoder);                                                                             \
    virtual void initWithDecoder(Decoder &decoder);

#define CODING_DEFINE(NAME)                                                                                            \
    FactoryRegister<NAME> NAME::factoryReg(#NAME);                                                                     \
    std::string NAME::encodeType() { return #NAME; }

#endif /* defined(__NanoGirl__ObjectFactory__) */
