#include <algorithm>
#include <cstdlib>

#include "fixed_block_memory_resource.hpp"

fixed_block_memory_resource::fixed_block_memory_resource(size_t total_size)
    : memory_chunk(std::malloc(total_size)), chunk_size(total_size), next_free(0) {
    if (!memory_chunk) {
        throw std::bad_alloc();
    }
}

fixed_block_memory_resource::~fixed_block_memory_resource() {
    std::free(memory_chunk);
}

void* fixed_block_memory_resource::do_allocate(size_t bytes, size_t alignment) {
    auto found = std::find_if(blocks_info.begin(), blocks_info.end(),
        [bytes](const Block& blk) {
            return !blk.is_used && blk.size >= bytes;
        });

    if (found != blocks_info.end()) {
        found->is_used = true;
        return found->addr;
    }

    if (next_free + bytes > chunk_size) {
        throw std::bad_alloc();
    }

    void* new_addr = static_cast<char*>(memory_chunk) + next_free;
    blocks_info.push_back({new_addr, bytes, true});
    next_free += bytes;
    return new_addr;
}

void fixed_block_memory_resource::do_deallocate(void* p, size_t bytes, size_t alignment) {
    auto found = std::find_if(blocks_info.begin(), blocks_info.end(),
        [p](const Block& blk) {
            return blk.addr == p && blk.is_used;
        });
    
    if (found != blocks_info.end()) {
        found->is_used = false;
    }
}

bool fixed_block_memory_resource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}