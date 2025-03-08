#pragma once
#include <cstddef>
#include <mutex>
#include "ULog.h"


class MemoryManager {
public:
    static MemoryManager& GetInstance() {
        static MemoryManager instance;
        return instance;
    }

    void AddAllocation(std::size_t size) {
        std::lock_guard<std::mutex> lock(mutex_);
        totalAllocationBytes += size;
        totalAllocationCount++;
    }
    void SubtractAllocation(std::size_t size) {
        std::lock_guard<std::mutex> lock(mutex_);
        totalAllocationBytes -= size;
        totalAllocationCount--;
    }

    uint32 GetTotalAllocationBytes() const { return totalAllocationBytes; }
    uint32 GetTotalAllocationCount() const { return totalAllocationCount; }

private:
    MemoryManager() : totalAllocationBytes(0), totalAllocationCount(0) {}
    ~MemoryManager() {}
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;

    uint32 totalAllocationBytes;
    uint32 totalAllocationCount;
    mutable std::mutex mutex_;
};

// Overloading
void* operator new(std::size_t size) {
    // delte에서 할당된 크기 알아내기 위해 헤더 추가
    std::size_t totalSize = size + sizeof(std::size_t);

    if (void* ptr = std::malloc(totalSize))
    {
        *((std::size_t*)ptr) = size;
        MemoryManager::GetInstance().AddAllocation(size);

        return static_cast<char*>(ptr) + sizeof(std::size_t);
    }

    throw std::bad_alloc();
}
void operator delete(void* ptr) noexcept {
    if (!ptr) return;

    void* originalPtr = static_cast<char*>(ptr) - sizeof(std::size_t);
    std::size_t size = *((std::size_t*)originalPtr);

    MemoryManager::GetInstance().SubtractAllocation(size);
    std::free(originalPtr);
}
