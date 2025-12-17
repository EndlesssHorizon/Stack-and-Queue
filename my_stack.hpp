#ifndef MY_STACK_HPP
#define MY_STACK_HPP

#include "abstract_seq.hpp"

template <typename T>
class MyStack : public AbstractSequence<T> {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d, Node* n) : data(d), next(n) {}
        Node(T&& d, Node* n) : data(std::move(d)), next(n) {}
    };

    Node* head_ = nullptr;
    size_t size_ = 0;

    void clear();
    void copy_from(const MyStack& other);

public:
    using Iterator = typename AbstractSequence<T>::Iterator;
    using ConstIterator = typename AbstractSequence<T>::ConstIterator;

    MyStack() = default;
    
    ~MyStack();
    MyStack(const MyStack& other);
    MyStack(MyStack&& other) noexcept;
    MyStack& operator=(const MyStack& other);
    MyStack& operator=(MyStack&& other) noexcept;
    
    MyStack& operator=(const AbstractSequence<T>& other) override;

    class StackIterImpl;
    class StackConstIterImpl;

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

#include "my_stack_impl.hpp"

#endif