#pragma once
class SCReadableBuffer
{
public:
    SCReadableBuffer();
    SCReadableBuffer(BYTE* InBuffer, uint32 InCapacity, uint32 InCurrentReadIndex = 0);
    ~SCReadableBuffer();

    BYTE* GetBuffer() { return Buffer; }
    uint32 GetCapacity() { return Capacity; }
    uint32 GetCurrentReadIndex() { return CurrentReadIndex; }
    uint32 GetSlot() { return Capacity - CurrentReadIndex; }

    template<typename T>
    bool Peek(T* InStartPtrToRead) { return Peek(InStartPtrToRead, sizeof(T)); }
    bool Peek(void* InStartPtrToRead, uint32 InReadLength);

    template<typename T>
    bool Read(T* InStartPtrToRead) { return Read(InStartPtrToRead, sizeof(T)); }
    bool Read(void* InStartPtrToRead, uint32 InReadLength);

    template<typename T>
    SCReadableBuffer& operator>>(OUT T& InStartPtrToRead);

private:
    BYTE* Buffer = nullptr;
    uint32 Capacity = 0;
    uint32 CurrentReadIndex = 0;

};

template<typename T>
inline SCReadableBuffer& SCReadableBuffer::operator>>(OUT T& InStartPtrToRead)
{
    InStartPtrToRead = *reinterpret_cast<T*>(&Buffer[CurrentReadIndex]);
    CurrentReadIndex += sizeof(T);
    return *this;
}
