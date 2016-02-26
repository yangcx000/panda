#ifndef MEMORY_H
#define MEMORY_H

union MemEntry {
    union MemEntry* m_next;
    char data[1]; 
};

enum {
    ALIGN_SIZE = 8,
    CHUNK_SIZE = 128
};

class MemAllocator {
    public:
        MemAllocator();
        ~MemAllocator();

        void Construct();
        void Destroy();

        void Allocate();
        void DeAllocate();

    private:
        MemEntry** m_arena;
};
#endif
