#include <gtest/gtest.h>

#include "fixed_block_memory_resource.hpp"
#include "stack.hpp"

TEST(StackTest, BasicOperations) {
    fixed_block_memory_resource pool(1024);
    stack<int> s(&pool);
    
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0);
    
    s.push(1);
    s.push(2);
    s.push(3);
    
    EXPECT_FALSE(s.empty());
    EXPECT_EQ(s.size(), 3);
    EXPECT_EQ(s.top(), 3);
    
    s.pop();
    EXPECT_EQ(s.top(), 2);
    EXPECT_EQ(s.size(), 2);
}

TEST(StackTest, Iterator) {
    fixed_block_memory_resource pool(1024);
    stack<int> s(&pool);
    
    s.push(1);
    s.push(2);
    s.push(3);
    
    std::vector<int> values;
    for (auto it = s.begin(); it != s.end(); ++it) {
        values.push_back(*it);
    }
    
    EXPECT_EQ(values, std::vector<int>({3, 2, 1}));
}

TEST(StackTest, ComplexType) {
    fixed_block_memory_resource pool(2048);
    stack<Point> s(&pool);
    
    s.push(Point(1, 2, 3));
    s.push(Point(4, 5, 6));
    
    EXPECT_EQ(s.top().x, 4);
    EXPECT_EQ(s.top().y, 5);
    EXPECT_EQ(s.top().z, 6);
    
    s.pop();
    EXPECT_EQ(s.top().x, 1);
}

TEST(StackTest, MemoryReuse) {
    fixed_block_memory_resource pool(256);
    
    void* prev_ptr = nullptr;
    {
        stack<int> s(&pool);
        s.push(42);
        prev_ptr = &s.top();
    }
    
    {
        stack<int> s(&pool);
        s.push(100);
        void* new_ptr = &s.top();
        EXPECT_EQ(prev_ptr, new_ptr);
    }
}

TEST(StackTest, Clear) {
    fixed_block_memory_resource pool(1024);
    stack<int> s(&pool);
    
    for (int i = 0; i < 5; ++i) {
        s.push(i);
    }
    
    EXPECT_EQ(s.size(), 5);
    s.clear();
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0);
}