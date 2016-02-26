#include "memory.h"
#include <cstddef>
#include <cstdio>

MemAllocator::MemAllocator()
    : m_arena(nullptr) {
}

MemAllocator::~MemAllocator() {
}

void MemAllocator::Construct() {
    m_arena = new MemEntry*[CHUNK_SIZE/ALIGN_SIZE];
    if (nullptr == m_arena) {
        fprintf(stderr, "Error: Construct mem pool failed\n");
        return;
    }
    
    for (int i = 0; i < CHUNK_SIZE / ALIGN_SIZE; ++i) {
        m_arena[i]->next = NULL;
    }
}

void MemAllocator::Allocate(size_t block_size) {
    
}

