cmake_minimum_required(VERSION 3.15)
project(pmr_stack)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pmr_stack 
    main.cpp 
    fixed_block_memory_resource.cpp
)
