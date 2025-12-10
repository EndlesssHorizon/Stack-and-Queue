#ifndef ABSTRACT_SEQ_HPP
#define ABSTRACT_SEQ_HPP

#include <iostream>
#include <stdexcept>
#include <iterator>
#include <utility>

// Абстрактный базовый класс
template <typename T>
class AbstractSequence {
public:
    // --- Внутренние абстрактные интерфейсы реализаций ---
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
        const T& dereference() const override {
            return const_cast<IteratorImplBase*>(this)->dereference(); 
        }
        virtual IteratorImplBase* clone() const override = 0;
    };

    // --- Обертки итераторов ---
    class Iterator;
    class ConstIterator;

    // --- Интерфейс контейнера ---
    virtual ~AbstractSequence() = default;

    virtual void push(const T& value) = 0;
    virtual void push(T&& value) = 0;
    virtual void pop() = 0;

    virtual const T& get_front() const = 0;
    virtual T& get_front() = 0;

    virtual bool is_empty() const = 0;
    virtual size_t size() const = 0;

    virtual Iterator begin() = 0;
    virtual Iterator end() = 0;
    virtual ConstIterator begin() const = 0;
    virtual ConstIterator end() const = 0;
    
    virtual ConstIterator cbegin() const { return begin(); }
    virtual ConstIterator cend() const { return end(); }
};

// =========================================================================
// Реализация итераторов
// =========================================================================

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
    explicit ConstIterator(ConstIteratorImplBase* impl) : impl_(impl) {}
    
    ConstIterator(const ConstIterator& other) 
        : impl_(other.impl_ ? other.impl_->clone() : nullptr) {}
    
    ConstIterator(ConstIterator&& other) noexcept : impl_(other.impl_) {
        other.impl_ = nullptr;
    }

    ~ConstIterator() { delete impl_; }

    ConstIterator& operator=(const ConstIterator& other);
    ConstIterator& operator=(ConstIterator&& other) noexcept;

    reference operator*() const;
    pointer operator->() const;

    ConstIterator& operator++();
    ConstIterator operator++(int);

    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const { return !(*this == other); }
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
    explicit Iterator(IteratorImplBase* impl) : impl_(impl) {}

    Iterator(const Iterator& other) 
        : impl_(other.impl_ ? other.impl_->clone() : nullptr) {}

    Iterator(Iterator&& other) noexcept : impl_(other.impl_) {
        other.impl_ = nullptr;
    }

    ~Iterator() { delete impl_; }

    Iterator& operator=(const Iterator& other);
    Iterator& operator=(Iterator&& other) noexcept;

    // Неявное преобразование в ConstIterator
    operator ConstIterator() const {
        return ConstIterator(impl_ ? impl_->clone() : nullptr);
    }

    reference operator*() const;
    pointer operator->() const;

    Iterator& operator++();
    Iterator operator++(int);

    // Сравнение Iterator == Iterator
    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const { return !(*this == other); }
    
    // Сравнение Iterator == ConstIterator
    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const { return !(*this == other); }
};

// Внешние операторы сравнения (ConstIterator == Iterator)
template <typename T>
bool operator==(const typename AbstractSequence<T>::ConstIterator& lhs, 
                const typename AbstractSequence<T>::Iterator& rhs) {
    return rhs == lhs;
}

template <typename T>
bool operator!=(const typename AbstractSequence<T>::ConstIterator& lhs, 
                const typename AbstractSequence<T>::Iterator& rhs) {
    return !(lhs == rhs);
}

// -------------------------------------------------------------------------
// Определения методов ConstIterator
// -------------------------------------------------------------------------

template <typename T>
auto AbstractSequence<T>::ConstIterator::operator=(const ConstIterator& other) -> ConstIterator& {
    if (this != &other) {
        ConstIteratorImplBase* new_impl = other.impl_ ? other.impl_->clone() : nullptr;
        delete impl_;
        impl_ = new_impl;
    }
    return *this;
}

template <typename T>
auto AbstractSequence<T>::ConstIterator::operator=(ConstIterator&& other) noexcept -> ConstIterator& {
    if (this != &other) {
        delete impl_;
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

template <typename T>
const T& AbstractSequence<T>::ConstIterator::operator*() const {
    if (!impl_) throw std::runtime_error("Dereferencing null iterator");
    return impl_->dereference();
}

template <typename T>
const T* AbstractSequence<T>::ConstIterator::operator->() const {
    if (!impl_) throw std::runtime_error("Dereferencing null iterator");
    return &impl_->dereference();
}

template <typename T>
auto AbstractSequence<T>::ConstIterator::operator++() -> ConstIterator& {
    if (!impl_) throw std::runtime_error("Incrementing null iterator");
    impl_->increment();
    return *this;
}

template <typename T>
auto AbstractSequence<T>::ConstIterator::operator++(int) -> ConstIterator {
    ConstIterator temp(*this);
    ++(*this);
    return temp;
}

template <typename T>
bool AbstractSequence<T>::ConstIterator::operator==(const ConstIterator& other) const {
    if (!impl_ && !other.impl_) return true;
    if (!impl_ || !other.impl_) return false;
    return impl_->equal(other.impl_);
}

// -------------------------------------------------------------------------
// Определения методов Iterator
// -------------------------------------------------------------------------

template <typename T>
auto AbstractSequence<T>::Iterator::operator=(const Iterator& other) -> Iterator& {
    if (this != &other) {
        IteratorImplBase* new_impl = other.impl_ ? other.impl_->clone() : nullptr;
        delete impl_;
        impl_ = new_impl;
    }
    return *this;
}

template <typename T>
auto AbstractSequence<T>::Iterator::operator=(Iterator&& other) noexcept -> Iterator& {
    if (this != &other) {
        delete impl_;
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

template <typename T>
T& AbstractSequence<T>::Iterator::operator*() const {
    if (!impl_) throw std::runtime_error("Dereferencing null iterator");
    return impl_->dereference();
}

template <typename T>
T* AbstractSequence<T>::Iterator::operator->() const {
    if (!impl_) throw std::runtime_error("Dereferencing null iterator");
    return &impl_->dereference();
}

template <typename T>
auto AbstractSequence<T>::Iterator::operator++() -> Iterator& {
    if (!impl_) throw std::runtime_error("Incrementing null iterator");
    impl_->increment();
    return *this;
}

template <typename T>
auto AbstractSequence<T>::Iterator::operator++(int) -> Iterator {
    Iterator temp(*this);
    ++(*this);
    return temp;
}

template <typename T>
bool AbstractSequence<T>::Iterator::operator==(const Iterator& other) const {
    if (!impl_ && !other.impl_) return true;
    if (!impl_ || !other.impl_) return false;
    return impl_->equal(other.impl_);
}

template <typename T>
bool AbstractSequence<T>::Iterator::operator==(const ConstIterator& other) const {
    ConstIterator temp = *this;
    return temp == other;
}

// --- Операторы потокового ввода-вывода ---

template <typename T>
std::ostream& operator<<(std::ostream& os, const AbstractSequence<T>& container) {
    bool first = true;
    for (auto it = container.begin(); it != container.end(); ++it) {
        if (!first) os << " ";
        os << *it;
        first = false;
    }
    return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, AbstractSequence<T>& container) {
    T value;
    while (is >> value) {
        container.push(value);
    }
    return is;
}

#endif