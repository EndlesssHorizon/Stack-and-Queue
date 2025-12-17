#ifndef MY_QUEUE_IMPL_HPP
#define MY_QUEUE_IMPL_HPP

// --- Реализация итераторов ---

template <typename T>
class MyQueue<T>::QueueIterImpl : public AbstractSequence<T>::IteratorImplBase {
    Node* current_;
public:
    QueueIterImpl(Node* node) : current_(node) {}
    void increment() override { if (current_) current_ = current_->next; }
    T& dereference() override { return current_->data; }
    bool equal(const typename AbstractSequence<T>::ConstIteratorImplBase* other) const override {
        const auto* casted = dynamic_cast<const QueueIterImpl*>(other);
        return casted && current_ == casted->current_;
    }
    typename AbstractSequence<T>::IteratorImplBase* clone() const override {
        return new QueueIterImpl(current_);
    }
};

template <typename T>
class MyQueue<T>::QueueConstIterImpl : public AbstractSequence<T>::ConstIteratorImplBase {
    const Node* current_;
public:
    QueueConstIterImpl(const Node* node) : current_(node) {}
    void increment() override { if (current_) current_ = current_->next; }
    const T& dereference() const override { return current_->data; }
    bool equal(const typename AbstractSequence<T>::ConstIteratorImplBase* other) const override {
        const auto* casted = dynamic_cast<const QueueConstIterImpl*>(other);
        return casted && current_ == casted->current_;
    }
    typename AbstractSequence<T>::ConstIteratorImplBase* clone() const override {
        return new QueueConstIterImpl(current_);
    }
};

// --- Методы MyQueue ---

template <typename T>
void MyQueue<T>::clear() {
    while (head_) {
        Node* temp = head_;
        head_ = head_->next;
        delete temp;
    }
    tail_ = nullptr;
    size_ = 0;
}

template <typename T>
void MyQueue<T>::copy_from(const MyQueue& other) {
    if (!other.head_) return;
    Node* curr = other.head_;
    while (curr) {
        push(curr->data); 
        curr = curr->next;
    }
}

template <typename T>
MyQueue<T>::~MyQueue() {
    clear();
}

template <typename T>
MyQueue<T>::MyQueue(const MyQueue& other) {
    copy_from(other);
}

template <typename T>
MyQueue<T>::MyQueue(MyQueue&& other) noexcept 
    : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
}

template <typename T>
MyQueue<T>& MyQueue<T>::operator=(const MyQueue& other) {
    if (this != &other) {
        clear();
        copy_from(other);
    }
    return *this;
}

template <typename T>
MyQueue<T>& MyQueue<T>::operator=(MyQueue&& other) noexcept {
    if (this != &other) {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename T>
MyQueue<T>& MyQueue<T>::operator=(const AbstractSequence<T>& other) {
    if (this != &other) {
        clear();
        for (auto it = other.begin(); it != other.end(); ++it) {
            push(*it);
        }
    }
    return *this;
}

template <typename T>
void MyQueue<T>::push(const T& value) {
    Node* newNode = new Node(value);
    if (!tail_) {
        head_ = tail_ = newNode;
    } else {
        tail_->next = newNode;
        tail_ = newNode;
    }
    ++size_;
}

template <typename T>
void MyQueue<T>::push(T&& value) {
    Node* newNode = new Node(std::move(value));
    if (!tail_) {
        head_ = tail_ = newNode;
    } else {
        tail_->next = newNode;
        tail_ = newNode;
    }
    ++size_;
}

template <typename T>
void MyQueue<T>::pop() {
    if (!head_) return;
    Node* temp = head_;
    head_ = head_->next;
    if (!head_) tail_ = nullptr;
    delete temp;
    --size_;
}

template <typename T>
T& MyQueue<T>::get_front() {
    if (!head_) throw std::out_of_range("Queue is empty");
    return head_->data;
}

template <typename T>
const T& MyQueue<T>::get_front() const {
    if (!head_) throw std::out_of_range("Queue is empty");
    return head_->data;
}

template <typename T>
bool MyQueue<T>::empty() const { 
    return head_ == nullptr; 
}

template <typename T>
size_t MyQueue<T>::size() const { 
    return size_; 
}

template <typename T>
typename MyQueue<T>::Iterator MyQueue<T>::begin() {
    return Iterator(new QueueIterImpl(head_));
}

template <typename T>
typename MyQueue<T>::Iterator MyQueue<T>::end() {
    return Iterator(new QueueIterImpl(nullptr));
}

template <typename T>
typename MyQueue<T>::ConstIterator MyQueue<T>::begin() const {
    return ConstIterator(new QueueConstIterImpl(head_));
}

template <typename T>
typename MyQueue<T>::ConstIterator MyQueue<T>::end() const {
    return ConstIterator(new QueueConstIterImpl(nullptr));
}

#endif