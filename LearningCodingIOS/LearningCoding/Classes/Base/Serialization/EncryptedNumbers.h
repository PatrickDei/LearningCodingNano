//
//  EncryptedNumbers.h
//  Hollywood
//
//  Created by Luka Piljek on 12/12/14.
//
//

#ifndef __Hollywood__EncryptedNumbers__
#define __Hollywood__EncryptedNumbers__

#include <cassert>
#include <cstdint>
#include <cstring>
#include <cstdlib>

class EncriptedInt
{
  public:
    EncriptedInt() { setValue(0); }

    EncriptedInt(int value) { setValue(value); }

    void setValue(int value) { _encValue = value xor getEncriptionKey(); }

    int getValue() const { return _encValue xor getEncriptionKey(); }

    unsigned int getEncriptionKey() const
    {
        if(_encriptionKey == 0) {
            _encriptionKey = rand() | 0x55;
        }
        return _encriptionKey;
    }

    operator int() const { return getValue(); }

    EncriptedInt &operator=(int value)
    {
        setValue(value);
        return *this;
    }

    EncriptedInt &operator=(EncriptedInt &value)
    {
        _encValue = value._encValue;
        return *this;
    }

  protected:
    static unsigned int _encriptionKey;
    unsigned int _encValue;
};

class EncriptedFloat
{
  public:
    EncriptedFloat() { setValue(0.0f); }

    EncriptedFloat(float value) { setValue(value); }

    void setValue(float value)
    {
        uint32_t temp;
        assert(sizeof temp == sizeof value); // reinterperting float data as int so
                                             // we can do bit operations on it
        memcpy(&temp, &value, sizeof temp);  // union would be a nicer way of doing
                                             // it, but the internet can't agree if
                                             // it's alowed by the standard
        _encValue = temp xor getEncriptionKey();
    }

    float getValue() const
    {
        uint32_t temp = _encValue xor getEncriptionKey();
        float f;
        assert(sizeof f == sizeof temp);
        memcpy(&f, &temp, sizeof f); // reinterperting int data as float
        return f;
    }

    uint32_t getEncriptionKey() const
    {
        if(_encriptionKey == 0) {
            _encriptionKey = rand() | 0x55;
        }
        return _encriptionKey;
    }

    operator float() const { return getValue(); }

    EncriptedFloat &operator=(float value)
    {
        setValue(value);
        return *this;
    }

    EncriptedFloat &operator=(EncriptedFloat &value)
    {
        _encValue = value._encValue;
        return *this;
    }

  protected:
    static uint32_t _encriptionKey;
    uint32_t _encValue;
};

class EncriptedDouble
{
  public:
    EncriptedDouble() { setValue(0.0); }

    EncriptedDouble(double value) { setValue(value); }

    void setValue(double value)
    {
        uint64_t temp;
        assert(sizeof temp == sizeof value); // reinterperting double data as int so
                                             // we can do bit operations on it
        memcpy(&temp, &value, sizeof temp);  // union would be a nicer way of doing
                                             // it, but the internet can't agree if
                                             // it's alowed by the standard
        _encValue = temp xor getEncriptionKey();
    }

    double getValue() const
    {
        uint64_t temp = _encValue xor getEncriptionKey();
        double f;
        assert(sizeof f == sizeof temp);
        memcpy(&f, &temp, sizeof f); // reinterperting int data as double
        return f;
    }

    uint64_t getEncriptionKey() const
    {
        if(_encriptionKey == 0) {
            _encriptionKey = rand() | 0x55;
        }
        return _encriptionKey;
    }

    operator double() const { return getValue(); }

    EncriptedDouble &operator=(double value)
    {
        setValue(value);
        return *this;
    }

    EncriptedDouble &operator=(EncriptedDouble &value)
    {
        _encValue = value._encValue;
        return *this;
    }

  protected:
    static uint64_t _encriptionKey;
    uint64_t _encValue;
};

#endif /* defined(__Hollywood__EncryptedNumbers__) */
