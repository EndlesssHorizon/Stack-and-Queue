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

public:
    MyStack() = default;
    
    ~MyStack() {
        while (!is_empty()) pop();
    }

    MyStack(const MyStack&) = delete;
    MyStack& operator=(const MyStack&) = delete;

    // --- Реализация итераторов ---

    class StackIterImpl : public AbstractSequence<T>::IteratorImplBase {
        Node* current_;
    public:
        StackIterImpl(Node* node) : current_(node) {}
        
        void increment() override {
            if (current_) current_ = current_->next;
        }
        
        T& dereference() const override {
            return current_->data;
        }
        
        bool equal(const typename AbstractSequence<T>::IteratorImplBase* other) const override {
            const auto* casted = dynamic_cast<const StackIterImpl*>(other);
            if (!casted) return false; 
            return current_ == casted->current_;
        }
        
        typename AbstractSequence<T>::IteratorImplBase* clone() const override {
            return new StackIterImpl(current_);
        }
        
        Node* get_node() const { return current_; }
    };

    class StackConstIterImpl : public AbstractSequence<T>::ConstIteratorImplBase {
        const Node* current_;
    public:
        StackConstIterImpl(const Node* node) : current_(node) {}
        
        void increment() override {
            if (current_) current_ = current_->next;
        }
        
        const T& dereference() const override {
            return current_->data;
        }
        
        bool equal(const typename AbstractSequence<T>::ConstIteratorImplBase* other) const override {
            const auto* casted = dynamic_cast<const StackConstIterImpl*>(other);
            if (!casted) return false;
            return current_ == casted->current_;
        }
        
        typename AbstractSequence<T>::ConstIteratorImplBase* clone() const override {
            return new StackConstIterImpl(current_);
        }
    };

    // --- Методы контейнера ---

    void push(const T& value) override {
        head_ = new Node(value, head_);
        ++size_;
    }

    void push(T&& value) override {
        head_ = new Node(std::move(value), head_);
        ++size_;
    }

    void pop() override {
        if (!head_) return;
        Node* temp = head_;
        head_ = head_->next;
        delete temp;
        --size_;
    }

    T& get_front() override {
        if (!head_) throw std::out_of_range("Stack is empty");
        return head_->data;
    }

    const T& get_front() const override {
        if (!head_) throw std::out_of_range("Stack is empty");
        return head_->data;
    }

    bool is_empty() const override { return head_ == nullptr; }
    size_t size() const override { return size_; }

    // --- Создание итераторов ---

    typename AbstractSequence<T>::Iterator begin() override {
        return typename AbstractSequence<T>::Iterator(new StackIterImpl(head_));
    }

    typename AbstractSequence<T>::Iterator end() override {
        return typename AbstractSequence<T>::Iterator(new StackIterImpl(nullptr));
    }

    typename AbstractSequence<T>::ConstIterator begin() const override {
        return typename AbstractSequence<T>::ConstIterator(new StackConstIterImpl(head_));
    }

    typename AbstractSequence<T>::ConstIterator end() const override {
        return typename AbstractSequence<T>::ConstIterator(new StackConstIterImpl(nullptr));
    }

    typename AbstractSequence<T>::ConstIterator cbegin() const override {
        return begin();
    }
    
    typename AbstractSequence<T>::ConstIterator cend() const override {
        return end();
    }
};

#endif