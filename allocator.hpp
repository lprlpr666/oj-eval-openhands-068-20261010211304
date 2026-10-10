#ifndef TLSF_ALLOCATOR_HPP
#define TLSF_ALLOCATOR_HPP

#include <cstddef>
#include <cstdint>
#include <array>

class TLSFAllocator {
public:
    explicit TLSFAllocator(std::size_t memoryPoolSize);
    ~TLSFAllocator();

    void* allocate(std::size_t size);
    void deallocate(void* ptr);

    void* getMemoryPoolStart() const;
    std::size_t getMemoryPoolSize() const;
    std::size_t getMaxAvailableBlockSize() const;

    TLSFAllocator(const TLSFAllocator&) = delete;
    TLSFAllocator& operator=(const TLSFAllocator&) = delete;
    TLSFAllocator(TLSFAllocator&&) = delete;
    TLSFAllocator& operator=(TLSFAllocator&&) = delete;

private:
    struct BlockHeader {
        std::size_t size; // total size including header
        bool isFree;
        BlockHeader* prevPhys;
        BlockHeader* nextPhys;
        BlockHeader* prevFree;
        BlockHeader* nextFree;
    };

    void* memoryPool;
    std::size_t poolSize;
    BlockHeader* freeListHead;

    static constexpr std::size_t headerSize = sizeof(BlockHeader);
    static constexpr std::size_t align = alignof(std::max_align_t);

    std::size_t alignUp(std::size_t n) const {
        return (n + align - 1) & ~(align - 1);
    }

    void insertFreeBlock(BlockHeader* block);
    void removeFreeBlock(BlockHeader* block);
    void splitBlock(BlockHeader* block, std::size_t neededSize);
    void mergeAdjacentFreeBlocks(BlockHeader* block);
};

#endif // TLSF_ALLOCATOR_HPP
