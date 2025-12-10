#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "abstract_seq.hpp"
#include "my_stack.hpp"
#include "my_queue.hpp"

TEST(StackTest, BasicOperationsInt) {
    MyStack<int> st;
    EXPECT_TRUE(st.is_empty());
    st.push(10);
    st.push(20);
    st.push(30);
    EXPECT_EQ(st.get_front(), 30);
    st.pop();
    EXPECT_EQ(st.get_front(), 20);
}

TEST(QueueTest, BasicOperationsString) {
    MyQueue<std::string> q;
    q.push("first");
    q.push("second");
    EXPECT_EQ(q.get_front(), "first");
    q.pop();
    EXPECT_EQ(q.get_front(), "second");
}

TEST(StackTest, Stack_Iterator_Complex) {
    MyStack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);

    MyStack<int>::ConstIterator cit = s.cbegin();
    EXPECT_EQ(*cit, 30);
    
    auto ocit = ++cit; 
    EXPECT_EQ(*cit, 20);
    EXPECT_EQ(*ocit, 20);
    
    ocit = cit++; 
    EXPECT_EQ(*cit, 10);
    EXPECT_EQ(*ocit, 20);
    
    ++cit;
    EXPECT_EQ(cit, s.cend());

    MyStack<int>::Iterator it = s.begin();
    EXPECT_EQ(*it, 30);
    *it = 35; 
    EXPECT_EQ(s.get_front(), 35);
}

TEST(StackTest, Stack_For_Loops) {
    MyStack<int> s;
    s.push(1);
    s.push(2);
    s.push(3); 

    const MyStack<int>& r = s;
    int expected1[] = {3, 2, 1};
    int idx = 0;
    for (auto& v : r) EXPECT_EQ(v, expected1[idx++]);

    for (auto& v : s) v *= 2; 

    int expected2[] = {6, 4, 2};
    idx = 0;
    for (const auto& v : s) EXPECT_EQ(v, expected2[idx++]);
}

TEST(StackTest, CopyAndMove) {
    MyStack<int> s;
    s.push(1);
    s.push(2); 

    MyStack<int> copy_s(s);
    EXPECT_EQ(copy_s.size(), 2);
    EXPECT_EQ(copy_s.get_front(), 2);

    s.pop(); 
    s.push(99); 

    EXPECT_EQ(s.get_front(), 99);
    EXPECT_EQ(copy_s.get_front(), 2);

    MyStack<int> s2;
    s2 = copy_s;
    EXPECT_EQ(s2.get_front(), 2);

    MyStack<int> moved_s(std::move(copy_s));
    EXPECT_EQ(moved_s.get_front(), 2);
    EXPECT_TRUE(copy_s.is_empty()); 

    MyStack<int> s3;
    s3 = std::move(s2);
    EXPECT_EQ(s3.get_front(), 2);
    EXPECT_TRUE(s2.is_empty());
}

TEST(QueueTest, CopyAndMove) {
    MyQueue<int> q;
    q.push(1);
    q.push(2);

    MyQueue<int> copy_q(q);
    q.push(3);

    EXPECT_EQ(q.size(), 3);
    EXPECT_EQ(copy_q.size(), 2);

    auto it = copy_q.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
}

TEST(StackTest, STL_Algorithms) {
    MyStack<int> s;
    for (int i = 1; i <= 5; ++i) s.push(i);

    auto it = std::find_if(s.begin(), s.end(), [](int v){ return v % 2 == 0; });
    EXPECT_NE(it, s.end());
    EXPECT_EQ(*it, 4);

    auto count = std::count_if(s.begin(), s.end(), [](int v){ return v % 2 == 0; });
    EXPECT_EQ(count, 2);
}

TEST(ContainerTest, Polymorphism) {
    MyStack<int> s;
    MyQueue<int> q;

    s.push(1); s.push(2);
    q.push(10); q.push(20); 

    AbstractSequence<int>& bs = s;
    EXPECT_EQ(bs.size(), 2);
    EXPECT_EQ(bs.get_front(), 2);

    s = q; 
    
    EXPECT_EQ(s.size(), 2);
    EXPECT_EQ(s.get_front(), 10);
    s.pop();
    EXPECT_EQ(s.get_front(), 20);
    
    MyStack<int> s2; 
    s2.push(5); s2.push(6); 
    q = s2;
    EXPECT_EQ(q.get_front(), 6);
}

TEST(StackTest, IO) {
    MyStack<int> s;
    std::stringstream sin("1 2 3");
    sin >> s; 
    
    std::stringstream sout;
    sout << s;
    EXPECT_EQ(sout.str(), "3 2 1");
}
