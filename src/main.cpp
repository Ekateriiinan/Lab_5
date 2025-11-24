#include <iostream>

#include "stack.hpp"
#include "fixed_block_memory_resource.hpp"

struct ComplexData {
    int id;
    double score;
    std::string tag;
    
    ComplexData(int i, double s, const std::string& t) : id(i), score(s), tag(t) {}
};

int main() {
    fixed_block_memory_resource mem_pool(2048);
    
    pmr_stack<int> int_stack(&mem_pool);
    int_stack.push(10);
    int_stack.push(20);
    int_stack.push(30);
    
    std::cout << "Integer stack: ";
    for (auto it = int_stack.begin(); it != int_stack.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    pmr_stack<ComplexData> data_stack(&mem_pool);
    data_stack.push(ComplexData(1, 9.5, "alpha"));
    data_stack.push(ComplexData(2, 8.7, "beta"));
    data_stack.push(ComplexData(3, 7.2, "gamma"));
    
    std::cout << "Complex data stack: ";
    for (const auto& item : data_stack) {
        std::cout << "{" << item.id << ", " << item.score << ", " << item.tag << "} ";
    }
    std::cout << std::endl;
    
    return 0;
}
