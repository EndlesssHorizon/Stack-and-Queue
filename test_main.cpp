#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>

#include "abstract_seq.hpp"
#include "my_stack.hpp"
#include "my_queue.hpp"

// Тест для проверки функциональности Стека (int)
TEST(StackTest, BasicOperationsInt) {
    MyStack<int> st;
    EXPECT_TRUE(st.is_empty());
    EXPECT_EQ(st.size(), 0);

    st.push(10);
    st.push(20);
    st.push(30);

    EXPECT_FALSE(st.is_empty());
    EXPECT_EQ(st.size(), 3);
    EXPECT_EQ(st.get_front(), 30);

    st.pop();
    EXPECT_EQ(st.get_front(), 20);
    EXPECT_EQ(st.size(), 2);
}

// Тест итераторов Стека
TEST(StackTest, IteratorCheck) {
    MyStack<int> st;
    st.push(1);
    st.push(2);
    st.push(3);

    // Ожидаемый порядок в стеке: 3, 2, 1
    std::vector<int> expected = {3, 2, 1};
    size_t idx = 0;

    for (auto it = st.begin(); it != st.end(); ++it) {
        EXPECT_EQ(*it, expected[idx++]);
    }

    // Проверка const итераторов
    const MyStack<int>& const_st = st;
    idx = 0;
    for (auto it = const_st.cbegin(); it != const_st.cend(); ++it) {
        EXPECT_EQ(*it, expected[idx++]);
    }
}

// Тест для проверки функциональности Очереди (string)
TEST(QueueTest, BasicOperationsString) {
    MyQueue<std::string> q;
    EXPECT_TRUE(q.is_empty());

    q.push("first");
    q.push("second");
    q.push("third");

    EXPECT_EQ(q.size(), 3);
    EXPECT_EQ(q.get_front(), "first");

    q.pop();
    EXPECT_EQ(q.get_front(), "second");
}

// Тест итераторов Очереди
TEST(QueueTest, IteratorCheck) {
    MyQueue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);

    std::vector<int> expected = {10, 20, 30};
    size_t idx = 0;

    for (auto val : q) {
        EXPECT_EQ(val, expected[idx++]);
    }
}

// Тест потокового ввода-вывода
TEST(StreamTest, OutputOperator) {
    MyStack<int> st;
    st.push(1);
    st.push(2);
    
    std::stringstream ss;
    ss << st;
    EXPECT_EQ(ss.str(), "2 1 ");
}

TEST(StreamTest, InputOperator) {
    MyQueue<int> q;
    std::stringstream ss("100 200");
    ss >> q;
    
    EXPECT_EQ(q.size(), 2);
    EXPECT_EQ(q.get_front(), 100);
    q.pop();
    EXPECT_EQ(q.get_front(), 200);
}

// Проверка сравнения итераторов
TEST(IteratorTest, Comparison) {
    MyStack<int> st;
    st.push(5);
    
    auto it1 = st.begin();
    auto it2 = st.begin();
    auto it_end = st.end();

    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 == it_end);
    
    ++it1;
    EXPECT_TRUE(it1 == it_end);
    EXPECT_TRUE(it1 != it2);
}

// Проверка работы с кастомным типом
struct Point {
    int x, y;
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "{" << p.x << "," << p.y << "}";
    }
};

TEST(CustomTypeTest, StackOfPoints) {
    MyStack<Point> st;
    st.push({1, 2});
    st.push({3, 4});

    EXPECT_EQ(st.get_front(), (Point{3, 4}));
    st.pop();
    EXPECT_EQ(st.get_front(), (Point{1, 2}));
}
