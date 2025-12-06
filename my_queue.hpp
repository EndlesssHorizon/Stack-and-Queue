#ifndef MY_QUEUE_HPP
#define MY_QUEUE_HPP

#include "abstract_seq.hpp"

template <typename T>
class MyQueue : public AbstractSequence<T> {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d, Node* n = nullptr) : data(d), next(n) {}
        Node(T&& d, Node* n = nullptr) : data(std::move(d)), next(n) {}
    };

    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    size_t size_ = 0;

public:
    MyQueue() = default;
    
    ~MyQueue() {
        while (!is_empty()) pop();
    }

    MyQueue(const MyQueue&) = delete;
    MyQueue& operator=(const MyQueue&) = delete;

    // --- Реализация итераторов ---
    
    class QueueIterImpl : public AbstractSequence<T>::IteratorImplBase {
        Node* current_;
    public:
        QueueIterImpl(Node* node) : current_(node) {}
        void increment() override { if (current_) current_ = current_->next; }
        T& dereference() const override { return current_->data; }
        bool equal(const typename AbstractSequence<T>::IteratorImplBase* other) const override {
            const auto* casted = dynamic_cast<const QueueIterImpl*>(other);
            return casted && current_ == casted->current_;
        }
        typename AbstractSequence<T>::IteratorImplBase* clone() const override {
            return new QueueIterImpl(current_);
        }
    };

    class QueueConstIterImpl : public AbstractSequence<T>::ConstIteratorImplBase {
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

    // --- Методы контейнера ---

    void push(const T& value) override {
        Node* newNode = new Node(value);
        if (!tail_) {
            head_ = tail_ = newNode;
        } else {
            tail_->next = newNode;
            tail_ = newNode;
        }
        ++size_;
    }

    void push(T&& value) override {
        Node* newNode = new Node(std::move(value));
        if (!tail_) {
            head_ = tail_ = newNode;
        } else {
            tail_->next = newNode;
            tail_ = newNode;
        }
        ++size_;
    }

    void pop() override {
        if (!head_) return;
        Node* temp = head_;
        head_ = head_->next;
        if (!head_) tail_ = nullptr;
        delete temp;
        --size_;
    }

    T& get_front() override {
        if (!head_) throw std::out_of_range("Queue is empty");
        return head_->data;
    }

    const T& get_front() const override {
        if (!head_) throw std::out_of_range("Queue is empty");
        return head_->data;
    }

    bool is_empty() const override { return head_ == nullptr; }
    size_t size() const override { return size_; }

    typename AbstractSequence<T>::Iterator begin() override {
        return typename AbstractSequence<T>::Iterator(new QueueIterImpl(head_));
    }
    typename AbstractSequence<T>::Iterator end() override {
        return typename AbstractSequence<T>::Iterator(new QueueIterImpl(nullptr));
    }
    typename AbstractSequence<T>::ConstIterator begin() const override {
        return typename AbstractSequence<T>::ConstIterator(new QueueConstIterImpl(head_));
    }
    typename AbstractSequence<T>::ConstIterator end() const override {
        return typename AbstractSequence<T>::ConstIterator(new QueueConstIterImpl(nullptr));
    }
    typename AbstractSequence<T>::ConstIterator cbegin() const override { return begin(); }
    typename AbstractSequence<T>::ConstIterator cend() const override { return end(); }
};

#endif