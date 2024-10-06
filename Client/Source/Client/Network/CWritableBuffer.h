#pragma once

class CLIENT_API CWritableBuffer
{
public:
	CWritableBuffer();
	CWritableBuffer(BYTE* InBuffer, uint32 InCapacity, uint32 InCurrentWriteIndex = 0);
	~CWritableBuffer();

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
	CWritableBuffer& operator<<(T&& InStartPtrToWrite);

private:
	BYTE* Buffer = nullptr;
	uint32 Capacity = 0;
	uint32 CurrentWriteIndex = 0;

};

template<typename T>
T* CWritableBuffer::Reserve()
{
	if (GetSlot() < sizeof(T))
		return nullptr;

	T* ret = reinterpret_cast<T*>(&Buffer[CurrentWriteIndex]);
	CurrentWriteIndex += sizeof(T);
	return ret;
}

template<typename T>
CWritableBuffer& CWritableBuffer::operator<<(T&& InStartPtrToWrite)
{
	using DataType = std::remove_reference_t<T>;
	*reinterpret_cast<DataType*>(&Buffer[CurrentWriteIndex]) = std::move(InStartPtrToWrite);
	CurrentWriteIndex += sizeof(T);
	return *this;
}
