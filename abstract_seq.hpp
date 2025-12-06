#ifndef ABSTRACT_SEQ_HPP
#define ABSTRACT_SEQ_HPP

#include <iostream>
#include <stdexcept>
#include <memory>
#include <compare> 

// Абстрактный базовый класс контейнера
template <typename T>
class AbstractSequence {
public:
    // --- Внутренние абстрактные классы реализации итераторов ---
    
    // База для реализации любого итератора
    struct IteratorImplBase {
        virtual ~IteratorImplBase() = default;
        virtual void increment() = 0;
        virtual T& dereference() const = 0;
        virtual bool equal(const IteratorImplBase* other) const = 0;
        virtual IteratorImplBase* clone() const = 0;
    };

    // База для константного итератора
    struct ConstIteratorImplBase {
        virtual ~ConstIteratorImplBase() = default;
        virtual void increment() = 0;
        virtual const T& dereference() const = 0;
        virtual bool equal(const ConstIteratorImplBase* other) const = 0;
        virtual ConstIteratorImplBase* clone() const = 0;
    };

    // --- Обертки итераторов ---

    // Предварительное объявление
    class ConstIterator; 

    class Iterator {
        IteratorImplBase* impl_ = nullptr;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        // Конструктор по умолчанию
        Iterator() : impl_(nullptr) {}

        // Конструктор, принимающий владение реализацией
        explicit Iterator(IteratorImplBase* impl) : impl_(impl) {}

        // Конструктор копирования
        Iterator(const Iterator& other) : impl_(other.impl_ ? other.impl_->clone() : nullptr) {}

        // Конструктор перемещения
        Iterator(Iterator&& other) noexcept : impl_(other.impl_) {
            other.impl_ = nullptr;
        }

        // Деструктор
        ~Iterator() { delete impl_; }

        // Оператор присваивания (Copy-and-Swap)
        Iterator& operator=(const Iterator& other) {
            if (this != &other) {
                Iterator temp(other);
                std::swap(impl_, temp.impl_);
            }
            return *this;
        }

        Iterator& operator=(Iterator&& other) noexcept {
            if (this != &other) {
                delete impl_;
                impl_ = other.impl_;
                other.impl_ = nullptr;
            }
            return *this;
        }

        // Доступ к элементам
        T& operator*() const {
            if (!impl_) throw std::runtime_error("Dereferencing null iterator");
            return impl_->dereference();
        }

        T* operator->() const {
            if (!impl_) throw std::runtime_error("Dereferencing null iterator");
            return &(impl_->dereference());
        }

        // Префиксный инкремент
        Iterator& operator++() {
            if (!impl_) throw std::runtime_error("Incrementing null iterator");
            impl_->increment();
            return *this;
        }

        // Постфиксный инкремент
        Iterator operator++(int) {
            Iterator temp(*this);
            ++(*this);
            return temp;
        }

        // Сравнение
        bool operator==(const Iterator& other) const {
            if (!impl_ && !other.impl_) return true;
            if (!impl_ || !other.impl_) return false;
            return impl_->equal(other.impl_);
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        // Дружественность для доступа к impl_ при конверсии
        friend class ConstIterator;
    };

    class ConstIterator {
        ConstIteratorImplBase* impl_ = nullptr;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        ConstIterator() : impl_(nullptr) {}
        
        explicit ConstIterator(ConstIteratorImplBase* impl) : impl_(impl) {}

        ConstIterator(const ConstIterator& other) 
            : impl_(other.impl_ ? other.impl_->clone() : nullptr) {}

        ConstIterator(ConstIterator&& other) noexcept : impl_(other.impl_) {
            other.impl_ = nullptr;
        }
        
        ~ConstIterator() { delete impl_; }

        ConstIterator& operator=(const ConstIterator& other) {
            if (this != &other) {
                ConstIterator temp(other);
                std::swap(impl_, temp.impl_);
            }
            return *this;
        }
        
        ConstIterator& operator=(ConstIterator&& other) noexcept {
             if (this != &other) {
                delete impl_;
                impl_ = other.impl_;
                other.impl_ = nullptr;
            }
            return *this;
        }

        const T& operator*() const {
            if (!impl_) throw std::runtime_error("Dereferencing null const_iterator");
            return impl_->dereference();
        }

        const T* operator->() const {
             if (!impl_) throw std::runtime_error("Dereferencing null const_iterator");
            return &(impl_->dereference());
        }

        ConstIterator& operator++() {
            if (!impl_) throw std::runtime_error("Incrementing null const_iterator");
            impl_->increment();
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator temp(*this);
            ++(*this);
            return temp;
        }

        bool operator==(const ConstIterator& other) const {
            if (!impl_ && !other.impl_) return true;
            if (!impl_ || !other.impl_) return false;
            return impl_->equal(other.impl_);
        }
        
        bool operator!=(const ConstIterator& other) const { return !(*this == other); }
    };

    // --- Абстрактные методы контейнера ---

    virtual ~AbstractSequence() = default;

    virtual void push(const T& value) = 0;
    virtual void push(T&& value) = 0;
    virtual void pop() = 0;

    virtual const T& get_front() const = 0;
    virtual T& get_front() = 0;

    virtual bool is_empty() const = 0;
    virtual size_t size() const = 0;

    // Фабричные методы для итераторов
    virtual Iterator begin() = 0;
    virtual Iterator end() = 0;
    virtual ConstIterator begin() const = 0;
    virtual ConstIterator end() const = 0;
    virtual ConstIterator cbegin() const = 0;
    virtual ConstIterator cend() const = 0;
};

// --- Операторы потокового ввода-вывода ---

template <typename T>
std::ostream& operator<<(std::ostream& os, const AbstractSequence<T>& container) {
    for (auto it = container.begin(); it != container.end(); ++it) {
        os << *it << " ";
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