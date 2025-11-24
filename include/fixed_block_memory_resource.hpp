#pragma once

#include <cstdlib>
#include <memory>
#include <vector>
#include <new>

class fixed_block_memory_resource : public std::pmr::memory_resource {
public:
    explicit fixed_block_memory_resource(size_t total_size);
    ~fixed_block_memory_resource();

    fixed_block_memory_resource(const fixed_block_memory_resource&) = delete;
    fixed_block_memory_resource& operator=(const fixed_block_memory_resource&) = delete;

private:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

    struct Block {
        void* addr;
        size_t size;
        bool is_used;
    };

    std::vector<Block> blocks_info;
    void* memory_chunk;
    size_t chunk_size;
    size_t next_free;
};
