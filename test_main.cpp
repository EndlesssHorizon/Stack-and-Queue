#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "abstract_seq.hpp"
#include "my_stack.hpp"
#include "my_queue.hpp"

// =================================================================
// Тесты для STACK
// =================================================================

TEST(StackTest, Stack_Iterator) {
    MyStack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);

    MyStack<int>::ConstIterator cit = s.cbegin(), ocit;
    EXPECT_EQ(*cit, 30);
    ocit = ++cit;
    EXPECT_EQ(*cit, 20);
    EXPECT_EQ(*ocit, 20);
    ocit = cit++;
    EXPECT_EQ(*cit, 10);
    EXPECT_EQ(*ocit, 20);
    ++cit;
    EXPECT_EQ(cit, s.cend());

    const MyStack<int>& r = s;
    cit = r.begin();
    EXPECT_EQ(*cit, 30);
    ++cit;
    EXPECT_EQ(*cit, 20);
    cit++;
    EXPECT_EQ(*cit, 10);
    ++cit;
    EXPECT_EQ(cit, r.end());

    MyStack<int>::Iterator it = s.begin(), oit;
    EXPECT_EQ(*it, 30);
    oit = ++it;
    EXPECT_EQ(*it, 20);
    EXPECT_EQ(*oit, 20);
    oit = it++;
    EXPECT_EQ(*it, 10);
    EXPECT_EQ(*oit, 20);
    *oit = 5;
    EXPECT_EQ(*it, 10);
    EXPECT_EQ(*oit, 5);
    ++it;
    EXPECT_EQ(it, s.end());

    std::stringstream sout;
    sout << s;
    EXPECT_EQ(sout.str(), "30 5 10");
}

TEST(StackTest, Stack_For) {
    MyStack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    const MyStack<int>& r = s;
    int expected1[] = {3, 2, 1};
    int idx = 0;
    for (auto& v : r) EXPECT_EQ(v, expected1[idx++]);

    idx = 0;
    for (auto& v : s) v *= 2; 
    
    int expected_after_mul[] = {6, 4, 2};
    idx = 0;
    for (const auto& v : s) EXPECT_EQ(v, expected_after_mul[idx++]);

    int expected3[] = {7, 5, 3}; 

    for (MyStack<int>::Iterator it = s.begin(); it != s.end(); ++it) {
        *it += 1;
    }
    idx = 0;
    for (MyStack<int>::ConstIterator it = s.begin(); it != s.end(); ++it)
        EXPECT_EQ(*it, expected3[idx++]);
}

TEST(StackTest, Stack_PushPopCopy) {
    MyStack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    s.pop();
    s.push(10);
    s.push(20); 

    MyStack<int> copy_s(s);

    int expected_orig[] = {20, 10, 2, 1};
    int idx = 0;
    for (auto v : copy_s) EXPECT_EQ(v, expected_orig[idx++]);

    copy_s.pop();
    copy_s.pop();
    copy_s.push(99);
    copy_s.push(98);
    copy_s.push(97);

    idx = 0;
    for (auto v : s) EXPECT_EQ(v, expected_orig[idx++]);

    int expected_copy[] = {97, 98, 99, 2, 1};
    idx = 0;
    for (auto v : copy_s) EXPECT_EQ(v, expected_copy[idx++]);

    MyStack<int> moved_s(std::move(copy_s));

    int expected_moved[] = {97, 98, 99, 2, 1};
    idx = 0;
    for (auto v : moved_s) EXPECT_EQ(v, expected_moved[idx++]);

    EXPECT_TRUE(copy_s.empty());

    MyStack<int> s2;
    s2 = s;
    idx = 0;
    for (auto v : s2) EXPECT_EQ(v, expected_orig[idx++]);

    s2.pop();
    s2.push(42);
    s2.push(99);

    idx = 0;
    for (auto v : s) EXPECT_EQ(v, expected_orig[idx++]);

    int expected_copy_asgn[] = {99, 42, 10, 2, 1};
    idx = 0;
    for (auto v : s2) EXPECT_EQ(v, expected_copy_asgn[idx++]);

    MyStack<int> s3;
    s3 = std::move(s2);
    idx = 0;
    for (auto v : s3) EXPECT_EQ(v, expected_copy_asgn[idx++]);

    EXPECT_TRUE(s2.empty());
}

TEST(StackTest, Stack_IO) {
    MyStack<int> s;
    s.push(0);

    std::stringstream sin("1 2 3 4 5");
    sin >> s; 

    EXPECT_EQ(s.size(), 6);

    int expected[] = {5, 4, 3, 2, 1, 0};
    int idx = 0;
    for (auto v : s) EXPECT_EQ(v, expected[idx++]);

    std::stringstream sout;
    sout << s;

    std::string expected_str = "5 4 3 2 1 0";
    EXPECT_EQ(sout.str(), expected_str);
}

TEST(StackTest, Stack_Algs) {
    MyStack<int> s;
    for (int i = 1; i <= 5; ++i) s.push(i);

    auto it = std::find_if(s.begin(), s.end(), [](int v){ return v % 2 == 0; });
    EXPECT_EQ(*it, 4);
    *it = 3;

    const MyStack<int>& r = s;
    auto cit = std::find_if(r.begin(), r.end(), [](int v){ return v % 2 == 0; });
    EXPECT_EQ(*cit, 2);

    it = std::find_if(s.begin(), s.end(), [](int v){ return v == 0; });
    EXPECT_EQ(it, s.end());

    auto count_even = std::count_if(s.begin(), s.end(), [](int v){ return v % 2 == 0; });
    EXPECT_EQ(count_even, 1);
}

// =================================================================
// Тесты для QUEUE
// =================================================================

TEST(QueueTest, Queue_Iterator) {
    MyQueue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);

    MyQueue<int>::ConstIterator cit = q.cbegin(), ocit;
    EXPECT_EQ(*cit, 10);
    ocit = ++cit; 
    EXPECT_EQ(*cit, 20);
    EXPECT_EQ(*ocit, 20);
    
    ocit = cit++;
    EXPECT_EQ(*cit, 30);  
    EXPECT_EQ(*ocit, 20); 
    
    ++cit;
    EXPECT_EQ(cit, q.cend());

    MyQueue<int>::Iterator it = q.begin();
    EXPECT_EQ(*it, 10);
    *it = 15; 
    EXPECT_EQ(q.get_front(), 15);
    
    it++;
    EXPECT_EQ(*it, 20);
}

TEST(QueueTest, Queue_For) {
    MyQueue<int> q;
    q.push(1);
    q.push(2);
    q.push(3); 

    int expected[] = {1, 2, 3};
    int idx = 0;
    for (auto v : q) EXPECT_EQ(v, expected[idx++]);

    for (auto& v : q) v *= 10;

    int expected_mod[] = {10, 20, 30};
    idx = 0;
    for (const auto& v : q) EXPECT_EQ(v, expected_mod[idx++]);
}

TEST(QueueTest, Queue_PushPopCopy) {
    MyQueue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

    q.pop(); 
    q.push(10);
    q.push(20);

    MyQueue<int> copy_q(q);
    int expected[] = {2, 3, 10, 20};
    int idx = 0;
    for(auto v : copy_q) EXPECT_EQ(v, expected[idx++]);

    copy_q.pop();
    copy_q.pop();
    copy_q.push(99);

    idx = 0;
    for(auto v : q) EXPECT_EQ(v, expected[idx++]);

    int expected_mod[] = {10, 20, 99};
    idx = 0;
    for(auto v : copy_q) EXPECT_EQ(v, expected_mod[idx++]);

    MyQueue<int> moved_q(std::move(copy_q));
    EXPECT_TRUE(copy_q.empty());
    idx = 0;
    for(auto v : moved_q) EXPECT_EQ(v, expected_mod[idx++]);
}

TEST(QueueTest, Queue_IO) {
    MyQueue<int> q;
    q.push(0); 

    std::stringstream sin("1 2 3");
    sin >> q; 

    std::stringstream sout;
    sout << q;
    EXPECT_EQ(sout.str(), "0 1 2 3");
}

TEST(QueueTest, Queue_Algs) {
    MyQueue<int> q;
    for(int i=1; i<=5; ++i) q.push(i);

    auto it = std::find_if(q.begin(), q.end(), [](int x){ return x > 3; });
    EXPECT_NE(it, q.end());
    EXPECT_EQ(*it, 4);

    int odds = std::count_if(q.begin(), q.end(), [](int x){ return x % 2 != 0; });
    EXPECT_EQ(odds, 3);
}

TEST(ContainerTest, BaseContainer_Polymorphism) {
    MyStack<int> s;
    MyQueue<int> q;

    s.push(1); s.push(2); s.push(3); 
    q.push(10); q.push(20); q.push(30); 

    AbstractSequence<int>& bs = s;
    AbstractSequence<int>& bq = q;

    bs = q;
    
    int expected_q[] = {10, 20, 30};
    int idx = 0;
    for(auto v : s) EXPECT_EQ(v, expected_q[idx++]);

    bq = s;
    
    idx = 0;
    for(auto v : q) EXPECT_EQ(v, expected_q[idx++]);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}