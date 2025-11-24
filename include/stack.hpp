#pragma once

#include <cstdlib>
#include <iterator>
#include <memory>

template<typename T>
class pmr_stack {
private:
    struct Node {
        T value;
        Node* next_node;
        
        template<typename... Args>
        Node(Args&&... args, Node* next) 
            : value(std::forward<Args>(args)...), next_node(next) {}
    };

public:
    using allocator_type = std::pmr::polymorphic_allocator<Node>;
    
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator() : current_node(nullptr) {}
        explicit iterator(Node* node) : current_node(node) {}

        reference operator*() const { return current_node->value; }
        pointer operator->() const { return &current_node->value; }

        iterator& operator++() { current_node = current_node->next_node; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

        bool operator==(const iterator& other) const { return current_node == other.current_node; }
        bool operator!=(const iterator& other) const { return current_node != other.current_node; }

    private:
        Node* current_node;
    };

    pmr_stack() : pmr_stack(std::pmr::get_default_resource()) {}
    explicit pmr_stack(std::pmr::memory_resource* resource) 
        : node_alloc(resource), top_node(nullptr) {}

    ~pmr_stack() { clear(); }

    template<typename U>
    void push(U&& val) {
        Node* new_node = node_alloc.allocate(1);
        try {
            node_alloc.construct(new_node, std::forward<U>(val), top_node);
        } catch (...) {
            node_alloc.deallocate(new_node, 1);
            throw;
        }
        top_node = new_node;
    }

    void pop() {
        if (top_node) {
            Node* old_top = top_node;
            top_node = top_node->next_node;
            node_alloc.destroy(old_top);
            node_alloc.deallocate(old_top, 1);
        }
    }

    T& top() { return top_node->value; }
    const T& top() const { return top_node->value; }
    bool empty() const { return top_node == nullptr; }

    iterator begin() { return iterator(top_node); }
    iterator end() { return iterator(nullptr); }

    void clear() { while (top_node) pop(); }

private:
    allocator_type node_alloc;
    Node* top_node;
};
