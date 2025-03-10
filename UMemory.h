#pragma once
#include <cstddef>
#include <mutex>
#include <functional>
#include "ULog.h"


class UMemory {
public:
    static UMemory& GetInstance() {
        static UMemory instance;
        return instance;
    }

    void AddAllocation(std::size_t size) {
        totalAllocationBytes += size;
        totalAllocationCount++;
    }
    void SubtractAllocation(std::size_t size) {
        totalAllocationBytes -= size;
        totalAllocationCount--;
    }

    size_t GetTotalAllocationBytes() const { return totalAllocationBytes; }
    size_t GetTotalAllocationCount() const { return totalAllocationCount; }

    void RecordAllocationCheckpoint() {
        checkPoint = totalAllocationBytes;
    }

    size_t GetAllocationDeltaSinceCheckpoint() const {
        return totalAllocationBytes - checkPoint;
    }

    size_t TrackAllocationDelta(const std::function<void()>& spawnFunc) {
        RecordAllocationCheckpoint();
        spawnFunc();
        return GetAllocationDeltaSinceCheckpoint();
    }

private:
    UMemory() : totalAllocationBytes(0), totalAllocationCount(0), checkPoint(0){}
    ~UMemory() {}
    UMemory(const UMemory&) = delete;
    UMemory& operator=(const UMemory&) = delete;

    size_t totalAllocationBytes;
    int totalAllocationCount;
    size_t checkPoint;
};

// Overloading
void* operator new(std::size_t size) {
    // delete���� �Ҵ�� ũ�� �˾Ƴ��� ���� ��� �߰�
    std::size_t totalSize = size + sizeof(std::size_t);

    if (void* ptr = std::malloc(totalSize))
    {
        *((std::size_t*)ptr) = size;
        UMemory::GetInstance().AddAllocation(size);

        return static_cast<char*>(ptr) + sizeof(std::size_t);
    }

    throw std::bad_alloc();
}
void operator delete(void* ptr) noexcept {
    if (!ptr) return;

    void* originalPtr = static_cast<char*>(ptr) - sizeof(std::size_t);
    std::size_t size = *((std::size_t*)originalPtr);

    UMemory::GetInstance().SubtractAllocation(size);
    std::free(originalPtr);
}
