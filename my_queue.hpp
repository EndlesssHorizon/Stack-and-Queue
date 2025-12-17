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

    void clear();
    void copy_from(const MyQueue& other);

public:
    using Iterator = typename AbstractSequence<T>::Iterator;
    using ConstIterator = typename AbstractSequence<T>::ConstIterator;

    MyQueue() = default;
    
    ~MyQueue();
    MyQueue(const MyQueue& other);
    MyQueue(MyQueue&& other) noexcept;
    MyQueue& operator=(const MyQueue& other);
    MyQueue& operator=(MyQueue&& other) noexcept;
    
    MyQueue& operator=(const AbstractSequence<T>& other) override;

    class QueueIterImpl;
    class QueueConstIterImpl;

    void push(const T& value) override;
    void push(T&& value) override;
    void pop() override;

    T& get_front() override;
    const T& get_front() const override;

    bool empty() const override;
    size_t size() const override;

    Iterator begin() override;
    Iterator end() override;
    ConstIterator begin() const override;
    ConstIterator end() const override;
};

#include "my_queue_impl.hpp"

#endif