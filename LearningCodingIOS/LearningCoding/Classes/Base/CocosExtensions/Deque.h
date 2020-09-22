/****************************************************************************
Copyright (c) 2010 ForzeField Studios S.L. http://forzefield.com
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2014 Chukong Technologies

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __CCDEQUE_H__
#define __CCDEQUE_H__

#include "CCObject.h"
#include "ccMacros.h"
#include <algorithm> // for std::find
#include <deque>
#include <functional>

NS_CC_BEGIN

template <class T = CCObject *> class CC_DLL Deque : public Serializable
{
  public:
    // ------------------------------------------
    // Iterators
    // ------------------------------------------
    typedef typename std::deque<T>::iterator iterator;
    typedef typename std::deque<T>::const_iterator const_iterator;

    typedef typename std::deque<T>::reverse_iterator reverse_iterator;
    typedef typename std::deque<T>::const_reverse_iterator const_reverse_iterator;

    iterator begin() { return _data.begin(); }
    const_iterator begin() const { return _data.begin(); }

    iterator end() { return _data.end(); }
    const_iterator end() const { return _data.end(); }

    const_iterator cbegin() const { return _data.cbegin(); }
    const_iterator cend() const { return _data.cend(); }

    reverse_iterator rbegin() { return _data.rbegin(); }
    const_reverse_iterator rbegin() const { return _data.rbegin(); }

    reverse_iterator rend() { return _data.rend(); }
    const_reverse_iterator rend() const { return _data.rend(); }

    const_reverse_iterator crbegin() const { return _data.crbegin(); }
    const_reverse_iterator crend() const { return _data.crend(); }

    /** Constructor */
    Deque<T>() : _data()
    {
        static_assert(std::is_convertible<T, CCObject *>::value, "Invalid Type for cocos2d::Deque<T>!");
    }

    /** Constructor with a capacity */
    explicit Deque<T>(ssize_t capacity) : _data()
    {
        static_assert(std::is_convertible<T, CCObject *>::value, "Invalid Type for cocos2d::Deque<T>!");
        CCLOGINFO("In the default constructor with capacity of Deque.");
        reserve(capacity);
    }

    /** Destructor */
    ~Deque<T>()
    {
        CCLOGINFO("In the destructor of Deque.");
        clear();
    }

    /** Copy constructor */
    Deque<T>(const Deque<T> &other)
    {
        static_assert(std::is_convertible<T, CCObject *>::value, "Invalid Type for cocos2d::Deque<T>!");
        CCLOGINFO("In the copy constructor!");
        _data = other._data;
        addRefForAllObjects();
    }

    /** Move constructor */
    Deque<T>(Deque<T> &&other)
    {
        static_assert(std::is_convertible<T, CCObject *>::value, "Invalid Type for cocos2d::Deque<T>!");
        CCLOGINFO("In the move constructor of Deque!");
        _data = std::move(other._data);
    }

    /** Copy assignment operator */
    Deque<T> &operator=(const Deque<T> &other)
    {
        if(this != &other) {
            CCLOGINFO("In the copy assignment operator!");
            clear();
            _data = other._data;
            addRefForAllObjects();
        }
        return *this;
    }

    /** Move assignment operator */
    Deque<T> &operator=(Deque<T> &&other)
    {
        if(this != &other) {
            CCLOGINFO("In the move assignment operator!");
            clear();
            _data = std::move(other._data);
        }
        return *this;
    }

    /** Coding */
    virtual void encode(Encoder &encoder)
    {
        encoder.beginArray();
        for(auto object : _data) {
            encoder.add(object);
        }
        encoder.endArray();
        encoder.addEncoded("data");
    }

    virtual void initWithDecoder(Decoder &decoder)
    {
        decoder.getEncoded("data");

        while(decoder.getEncoded()) {
            CCObject *element;
            decoder.decodeData(element);
            if(element)
                _data.push_back(static_cast<T>(element));
        }
        decoder.popData();
    }

    // Don't uses operator since we could not decide whether it needs
    // 'retain'/'release'.
    //    T& operator[](int index)
    //    {
    //        return _data[index];
    //    }
    //
    //    const T& operator[](int index) const
    //    {
    //        return _data[index];
    //    }

    /** @brief Request a change in capacity
     *  @param capacity Minimum capacity for the deque.
     *         If n is greater than the current deque capacity,
     *         the function causes the container to reallocate its storage
     * increasing its capacity to n (or greater).
     */
    void reserve(ssize_t n) { _data.reserve(n); }

    /** @brief Returns the size of the storage space currently allocated for the
     * deque, expressed in terms of elements.
     *  @note This capacity is not necessarily equal to the deque size.
     *        It can be equal or greater, with the extra space allowing to
     * accommodate for growth without the need to reallocate on each insertion.
     *  @return The size of the currently allocated storage capacity in the deque,
     * measured in terms of the number elements it can hold.
     */
    ssize_t capacity() const { return _data.capacity(); }

    /** @brief Returns the number of elements in the deque.
     *  @note This is the number of actual objects held in the deque, which is not
     * necessarily equal to its storage capacity.
     *  @return The number of elements in the container.
     */
    ssize_t size() const { return _data.size(); }

    /** @brief Returns whether the deque is empty (i.e. whether its size is 0).
     *  @note This function does not modify the container in any way. To clear the
     * content of a deque, see Deque<T>::clear.
     */
    bool empty() const { return _data.empty(); }

    /** Returns the maximum number of elements that the deque can hold. */
    ssize_t max_size() const { return _data.max_size(); }

    /** Returns index of a certain object, return UINT_MAX if doesn't contain the
     * object */
    ssize_t get_index(T object) const
    {
        auto iter = std::find(_data.begin(), _data.end(), object);
        if(iter != _data.end())
            return iter - _data.begin();

        return -1;
    }

    /** @brief Find the object in the deque.
     *  @return Returns an iterator to the first element in the range [first,last)
     * that compares equal to val.
     *          If no such element is found, the function returns last.
     */
    const_iterator find(T object) const { return std::find(_data.begin(), _data.end(), object); }

    iterator find(T object) { return std::find(_data.begin(), _data.end(), object); }

    /** Returns the element at position 'index' in the deque. */
    T at(ssize_t index) const
    {
        CCAssert(index >= 0 && index < size(), "index out of range in getObjectAtIndex()");
        return _data[index];
    }

    /** Returns the first element in the deque. */
    T front() const { return _data.front(); }

    /** Returns the last element of the deque. */
    T back() const { return _data.back(); }

    /** Returns a Boolean value that indicates whether object is present in deque.
     */
    bool contains(T object) const { return (std::find(_data.begin(), _data.end(), object) != _data.end()); }

    /** Returns true if the two deques are equal */
    bool equals(const Deque<T> &other)
    {
        ssize_t s = this->size();
        if(s != other.size())
            return false;

        for(ssize_t i = 0; i < s; i++) {
            if(this->at(i) != other.at(i)) {
                return false;
            }
        }
        return true;
    }

    // Adds objects

    /** @brief Adds a new element at the end of the deque, after its current last
     * element.
     *  @note This effectively increases the container size by one,
     *        which causes an automatic reallocation of the allocated storage
     * space
     *        if -and only if- the new deque size surpasses the current deque
     * capacity.
     */
    void push_back(T object)
    {
        CCAssert(object != nullptr, "The object should not be nullptr");
        _data.push_back(object);
        object->retain();
    }

    /** Push all elements of an existing deque to the end of current deque. */
    void push_back(const Deque<T> &other)
    {
        for(const auto &obj : other) {
            _data.push_back(obj);
            obj->retain();
        }
    }

    /** @brief Adds a new element at the front of the deque, before its current
     * first element. */
    void push_front(T object)
    {
        CCAssert(object != nullptr, "The object should not be nullptr");
        _data.push_front(object);
        object->retain();
    }

    /** @brief Insert a certain object at a certain index
     *  @note The deque is extended by inserting new elements before the element
     * at the specified 'index',
     *        effectively increasing the container size by the number of elements
     * inserted.
     *        This causes an automatic reallocation of the allocated storage space
     *        if -and only if- the new deque size surpasses the current deque
     * capacity.
     */
    void insert(ssize_t index, T object)
    {
        CCAssert(index >= 0 && index <= size(), "Invalid index!");
        CCAssert(object != nullptr, "The object should not be nullptr");
        _data.insert((std::begin(_data) + index), object);
        object->retain();
    }

    // Removes Objects

    /** Removes the last element in the deque,
     *  effectively reducing the container size by one, decrease the referece
     * count of the deleted object.
     */
    void pop_back()
    {
        CCAssert(!_data.empty(), "no objects added");
        auto last = _data.back();
        _data.pop_back();
        last->release();
    }

    /** Removes the first element in the deque */
    void pop_front()
    {
        CCAssert(!_data.empty(), "no objects added");
        auto last = _data.front();
        _data.pop_front();
        last->release();
    }

    /** @brief Remove a certain object in Deque.
     *  @param object The object to be removed.
     *  @param removeAll Whether to remove all elements with the same value.
     *                   If its value is 'false', it will just erase the first
     * occurrence.
     */
    void erase_object(T object, bool removeAll = false)
    {
        CCAssert(object != nullptr, "The object should not be nullptr");

        if(removeAll) {
            for(auto iter = _data.begin(); iter != _data.end();) {
                if((*iter) == object) {
                    iter = _data.erase(iter);
                    object->release();
                } else {
                    ++iter;
                }
            }
        } else {
            auto iter = std::find(_data.begin(), _data.end(), object);
            if(iter != _data.end()) {
                _data.erase(iter);
                object->release();
            }
        }
    }

    /** @brief Removes from the deque with an iterator.
     *  @param position Iterator pointing to a single element to be removed from
     * the deque.
     *  @return An iterator pointing to the new location of the element that
     * followed the last element erased by the function call.
     *          This is the container end if the operation erased the last element
     * in the sequence.
     */
    iterator erase(iterator position)
    {
        CCAssert(position >= _data.begin() && position < _data.end(), "Invalid position!");
        (*position)->release();
        return _data.erase(position);
    }

    /** @brief Removes from the deque with a range of elements (  [first, last) ).
     *  @param first The beginning of the range
     *  @param last The end of the range, the 'last' will not used, it's only for
     * indicating the end of range.
     *  @return An iterator pointing to the new location of the element that
     * followed the last element erased by the function call.
     *          This is the container end if the operation erased the last element
     * in the sequence.
     */
    iterator erase(iterator first, iterator last)
    {
        for(auto iter = first; iter != last; ++iter) {
            (*iter)->release();
        }

        return _data.erase(first, last);
    }

    /** @brief Removes from the deque with an index.
     *  @param index The index of the element to be removed from the deque.
     *  @return An iterator pointing to the new location of the element that
     * followed the last element erased by the function call.
     *          This is the container end if the operation erased the last element
     * in the sequence.
     */
    iterator erase(ssize_t index)
    {
        CCAssert(!_data.empty() && index >= 0 && index < size(), "Invalid index!");
        auto it = std::next(begin(), index);
        (*it)->release();
        return _data.erase(it);
    }

    /** @brief Removes all elements from the deque (which are destroyed), leaving
     * the container with a size of 0.
     *  @note All the elements in the deque will be released (referece count will
     * be decreased).
     */
    void clear()
    {
        for(auto it = std::begin(_data); it != std::end(_data); ++it) {
            (*it)->release();
        }
        _data.clear();
    }

    // Rearranging Content

    /** Swap two elements */
    void swap(T object1, T object2)
    {
        ssize_t idx1 = getIndex(object1);
        ssize_t idx2 = getIndex(object2);

        CCAssert(idx1 >= 0 && idx2 >= 0, "invalid object index");

        std::swap(_data[idx1], _data[idx2]);
    }

    /** Swap two elements with certain indexes */
    void swap(ssize_t index1, ssize_t index2)
    {
        CCAssert(index1 >= 0 && index1 < size() && index2 >= 0 && index2 < size(), "Invalid indices");

        std::swap(_data[index1], _data[index2]);
    }

    /** Replace object at index with another object. */
    void replace(ssize_t index, T object)
    {
        CCAssert(index >= 0 && index < size(), "Invalid index!");
        CCAssert(object != nullptr, "The object should not be nullptr");

        _data[index]->release();
        _data[index] = object;
        object->retain();
    }

    /** reverses the deque */
    void reverse() { std::reverse(std::begin(_data), std::end(_data)); }

    /** Shrinks the deque so the memory footprint corresponds with the number of
     * items */
    void shrink_to_fit() { _data.shrink_to_fit(); }

  protected:
    /** Retains all the objects in the deque */
    void addRefForAllObjects()
    {
        for(const auto &obj : _data) {
            obj->retain();
        }
    }

    std::deque<T> _data;
};

// end of data_structure group
/// @}

NS_CC_END

#endif // __CCDEQUE_H__
