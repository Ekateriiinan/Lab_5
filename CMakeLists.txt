cmake_minimum_required(VERSION 3.15)
project(CustomStack)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основная библиотека
add_library(custom_stack
    fixed_block_memory_resource.cpp
    stack.cpp
)

# Демонстрационная программа
add_executable(demo demo.cpp)
target_link_libraries(demo custom_stack)

# Тесты
find_package(GTest REQUIRED)
add_executable(stack_tests test_stack.cpp)
target_link_libraries(stack_tests custom_stack GTest::gtest GTest::gtest_main)

# Включить тесты
include(GoogleTest)
gtest_discover_tests(stack_tests)