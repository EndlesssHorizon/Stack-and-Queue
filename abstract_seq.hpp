#ifndef ABSTRACT_SEQ_HPP
#define ABSTRACT_SEQ_HPP

#include <iostream>
#include <stdexcept>
#include <iterator>
#include <utility>

template <typename T>
class AbstractSequence {
public:
    struct ConstIteratorImplBase {
        virtual ~ConstIteratorImplBase() = default;
        virtual void increment() = 0;
        virtual const T& dereference() const = 0;
        virtual bool equal(const ConstIteratorImplBase* other) const = 0;
        virtual ConstIteratorImplBase* clone() const = 0;
    };

    struct IteratorImplBase : public ConstIteratorImplBase {
        virtual ~IteratorImplBase() = default;
        virtual T& dereference() = 0;
        const T& dereference() const override; 
        virtual IteratorImplBase* clone() const override = 0;
    };

    class Iterator;
    class ConstIterator;

    virtual ~AbstractSequence() = default;

    virtual AbstractSequence& operator=(const AbstractSequence& other) = 0;

    virtual void push(const T& value) = 0;
    virtual void push(T&& value) = 0;
    virtual void pop() = 0;

    virtual const T& get_front() const = 0;
    virtual T& get_front() = 0;

    virtual bool empty() const = 0;
    virtual size_t size() const = 0;

    virtual Iterator begin() = 0;
    virtual Iterator end() = 0;
    virtual ConstIterator begin() const = 0;
    virtual ConstIterator end() const = 0;
    
    virtual ConstIterator cbegin() const { return begin(); }
    virtual ConstIterator cend() const { return end(); }
};

template <typename T>
class AbstractSequence<T>::ConstIterator {
    friend class AbstractSequence<T>;
    friend class Iterator; 
protected:
    ConstIteratorImplBase* impl_ = nullptr;
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    ConstIterator() = default;
    explicit ConstIterator(ConstIteratorImplBase* impl);
    ConstIterator(const ConstIterator& other);
    ConstIterator(ConstIterator&& other) noexcept;
    ~ConstIterator();

    ConstIterator& operator=(const ConstIterator& other);
    ConstIterator& operator=(ConstIterator&& other) noexcept;

    reference operator*() const;
    pointer operator->() const;

    ConstIterator& operator++();
    ConstIterator operator++(int);

    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;
};

template <typename T>
class AbstractSequence<T>::Iterator {
    friend class AbstractSequence<T>;
    friend class ConstIterator; 
protected:
    IteratorImplBase* impl_ = nullptr;
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    Iterator() = default;
    explicit Iterator(IteratorImplBase* impl);
    Iterator(const Iterator& other);
    Iterator(Iterator&& other) noexcept;
    ~Iterator();

    Iterator& operator=(const Iterator& other);
    Iterator& operator=(Iterator&& other) noexcept;

    operator ConstIterator() const;

    reference operator*() const;
    pointer operator->() const;

    Iterator& operator++();
    Iterator operator++(int);

    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;
    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;
};

template <typename T>
bool operator==(const typename AbstractSequence<T>::ConstIterator& lhs, 
                const typename AbstractSequence<T>::Iterator& rhs);

template <typename T>
bool operator!=(const typename AbstractSequence<T>::ConstIterator& lhs, 
                const typename AbstractSequence<T>::Iterator& rhs);

template <typename T>
std::ostream& operator<<(std::ostream& os, const AbstractSequence<T>& container);

template <typename T>
std::istream& operator>>(std::istream& is, AbstractSequence<T>& container);

#include "abstract_seq_impl.hpp"

#endif