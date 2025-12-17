#ifndef MY_STACK_IMPL_HPP
#define MY_STACK_IMPL_HPP

// --- Реализация итераторов ---

template <typename T>
class MyStack<T>::StackIterImpl : public AbstractSequence<T>::IteratorImplBase {
    Node* current_;
public:
    StackIterImpl(Node* node) : current_(node) {}
    void increment() override { if (current_) current_ = current_->next; }
    T& dereference() override { return current_->data; }
    bool equal(const typename AbstractSequence<T>::ConstIteratorImplBase* other) const override {
        const auto* casted = dynamic_cast<const StackIterImpl*>(other);
        return casted && current_ == casted->current_;
    }
    typename AbstractSequence<T>::IteratorImplBase* clone() const override {
        return new StackIterImpl(current_);
    }
};

template <typename T>
class MyStack<T>::StackConstIterImpl : public AbstractSequence<T>::ConstIteratorImplBase {
    const Node* current_;
public:
    StackConstIterImpl(const Node* node) : current_(node) {}
    void increment() override { if (current_) current_ = current_->next; }
    const T& dereference() const override { return current_->data; }
    bool equal(const typename AbstractSequence<T>::ConstIteratorImplBase* other) const override {
        const auto* casted = dynamic_cast<const StackConstIterImpl*>(other);
        return casted && current_ == casted->current_;
    }
    typename AbstractSequence<T>::ConstIteratorImplBase* clone() const override {
        return new StackConstIterImpl(current_);
    }
};

// --- Методы MyStack ---

template <typename T>
void MyStack<T>::clear() {
    while (head_) {
        Node* temp = head_;
        head_ = head_->next;
        delete temp;
    }
    size_ = 0;
}

template <typename T>
void MyStack<T>::copy_from(const MyStack& other) {
    if (!other.head_) return;
    head_ = new Node(other.head_->data, nullptr);
    Node* current = head_;
    Node* other_curr = other.head_->next;
    while (other_curr) {
        current->next = new Node(other_curr->data, nullptr);
        current = current->next;
        other_curr = other_curr->next;
    }
    size_ = other.size_;
}

template <typename T>
MyStack<T>::~MyStack() {
    clear();
}

template <typename T>
MyStack<T>::MyStack(const MyStack& other) {
    copy_from(other);
}

template <typename T>
MyStack<T>::MyStack(MyStack&& other) noexcept 
    : head_(other.head_), size_(other.size_) {
    other.head_ = nullptr;
    other.size_ = 0;
}

template <typename T>
MyStack<T>& MyStack<T>::operator=(const MyStack& other) {
    if (this != &other) {
        clear();
        copy_from(other);
    }
    return *this;
}

template <typename T>
MyStack<T>& MyStack<T>::operator=(MyStack&& other) noexcept {
    if (this != &other) {
        clear();
        head_ = other.head_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename T>
MyStack<T>& MyStack<T>::operator=(const AbstractSequence<T>& other) {
    if (this != &other) {
        clear();
        auto copy_helper = [&](auto self, auto it, auto end) -> void {
            if (it == end) return;
            T val = *it;
            ++it;
            self(self, it, end);
            push(val);
        };
        copy_helper(copy_helper, other.begin(), other.end());
    }
    return *this;
}

template <typename T>
void MyStack<T>::push(const T& value) {
    head_ = new Node(value, head_);
    ++size_;
}

template <typename T>
void MyStack<T>::push(T&& value) {
    head_ = new Node(std::move(value), head_);
    ++size_;
}

template <typename T>
void MyStack<T>::pop() {
    if (!head_) return;
    Node* temp = head_;
    head_ = head_->next;
    delete temp;
    --size_;
}

template <typename T>
T& MyStack<T>::get_front() {
    if (!head_) throw std::out_of_range("Stack is empty");
    return head_->data;
}

template <typename T>
const T& MyStack<T>::get_front() const {
    if (!head_) throw std::out_of_range("Stack is empty");
    return head_->data;
}

template <typename T>
bool MyStack<T>::empty() const { 
    return head_ == nullptr; 
}

template <typename T>
size_t MyStack<T>::size() const { 
    return size_; 
}

template <typename T>
typename MyStack<T>::Iterator MyStack<T>::begin() {
    return Iterator(new StackIterImpl(head_));
}

template <typename T>
typename MyStack<T>::Iterator MyStack<T>::end() {
    return Iterator(new StackIterImpl(nullptr));
}

template <typename T>
typename MyStack<T>::ConstIterator MyStack<T>::begin() const {
    return ConstIterator(new StackConstIterImpl(head_));
}

template <typename T>
typename MyStack<T>::ConstIterator MyStack<T>::end() const {
    return ConstIterator(new StackConstIterImpl(nullptr));
}

#endif