#pragma once
class SCWritableBuffer
{
public:
    SCWritableBuffer();
    SCWritableBuffer(BYTE* InBuffer, uint32 InCapacity, uint32 InCurrentWriteIndex = 0);
    ~SCWritableBuffer();

    BYTE* GetBuffer() { return Buffer; }
    uint32 GetCapacity() { return Capacity; }
    uint32 GetCurrentWriteIndex() { return CurrentWriteIndex; }
    uint32 GetSlot() { return Capacity - CurrentWriteIndex; }

    template<typename T>
    bool Write(T* InStartPtrToWrite) { return Write(InStartPtrToWrite, sizeof(T)); }
    bool Write(void* InStartPtrToWrite, uint32 InWriteLength);

    template<typename T>
    T* Reserve();

    template<typename T>
    SCWritableBuffer& operator<<(const T& InStartPtrToWrite);

    template<typename T>
    SCWritableBuffer& operator<<(T&& InStartPtrToWrite);

private:
    BYTE* Buffer = nullptr;
    uint32 Capacity = 0;
    uint32 CurrentWriteIndex = 0;

};

template<typename T>
T* SCWritableBuffer::Reserve()
{
    if (GetFreeSize() < sizeof(T))
        return nullptr;

    T* ret = reinterpret_cast<T*>(&Buffer[CurrentWriteIndex]);
    CurrentWriteIndex += sizeof(T);
    return ret;
}

template<typename T>
SCWritableBuffer& SCWritableBuffer::operator<<(const T& InStartPtrToWrite)
{
    *reinterpret_cast<T*>(&Buffer[CurrentWriteIndex]) = InStartPtrToWrite;
    CurrentWriteIndex += sizeof(T);
    return *this;
}

template<typename T>
SCWritableBuffer& SCWritableBuffer::operator<<(T&& InStartPtrToWrite)
{
    *reinterpret_cast<T*>(&Buffer[CurrentWriteIndex]) = std::move(InStartPtrToWrite);
    CurrentWriteIndex += sizeof(T);
    return *this;
}
