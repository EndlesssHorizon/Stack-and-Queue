#ifndef ABSTRACT_SEQ_IMPL_HPP
#define ABSTRACT_SEQ_IMPL_HPP

// --- IteratorImplBase helper ---
template <typename T>
const T& AbstractSequence<T>::IteratorImplBase::dereference() const {
    return const_cast<IteratorImplBase*>(this)->dereference(); 
}

// --- ConstIterator Methods ---

template <typename T>
AbstractSequence<T>::ConstIterator::ConstIterator(ConstIteratorImplBase* impl) : impl_(impl) {}

template <typename T>
AbstractSequence<T>::ConstIterator::ConstIterator(const ConstIterator& other) 
    : impl_(other.impl_ ? other.impl_->clone() : nullptr) {}

template <typename T>
AbstractSequence<T>::ConstIterator::ConstIterator(ConstIterator&& other) noexcept 
    : impl_(other.impl_) {
    other.impl_ = nullptr;
}

template <typename T>
AbstractSequence<T>::ConstIterator::~ConstIterator() { delete impl_; }

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

template <typename T>
bool AbstractSequence<T>::ConstIterator::operator!=(const ConstIterator& other) const {
    return !(*this == other);
}

// --- Iterator Methods ---

template <typename T>
AbstractSequence<T>::Iterator::Iterator(IteratorImplBase* impl) : impl_(impl) {}

template <typename T>
AbstractSequence<T>::Iterator::Iterator(const Iterator& other) 
    : impl_(other.impl_ ? other.impl_->clone() : nullptr) {}

template <typename T>
AbstractSequence<T>::Iterator::Iterator(Iterator&& other) noexcept 
    : impl_(other.impl_) {
    other.impl_ = nullptr;
}

template <typename T>
AbstractSequence<T>::Iterator::~Iterator() { delete impl_; }

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
AbstractSequence<T>::Iterator::operator ConstIterator() const {
    return ConstIterator(impl_ ? impl_->clone() : nullptr);
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
bool AbstractSequence<T>::Iterator::operator!=(const Iterator& other) const { 
    return !(*this == other); 
}

template <typename T>
bool AbstractSequence<T>::Iterator::operator==(const ConstIterator& other) const {
    ConstIterator temp = *this;
    return temp == other;
}

template <typename T>
bool AbstractSequence<T>::Iterator::operator!=(const ConstIterator& other) const { 
    return !(*this == other); 
}

// --- Global Operators ---

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