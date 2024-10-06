#include "CWritableBuffer.h"

CWritableBuffer::CWritableBuffer()
{
}

CWritableBuffer::CWritableBuffer(BYTE* InBuffer, uint32 InCapacity, uint32 InCurrentWriteIndex)
	: Buffer(InBuffer)
	, Capacity(InCapacity)
	, CurrentWriteIndex(InCurrentWriteIndex)
{

}

CWritableBuffer::~CWritableBuffer()
{
}

bool CWritableBuffer::Write(void* InStartPtrToWrite, uint32 InWriteLength)
{
	if (GetSlot() < InWriteLength)
	{
		return false;
	}

	::memcpy(&Buffer[CurrentWriteIndex], InStartPtrToWrite, InWriteLength);
	CurrentWriteIndex += InWriteLength;
	return true;
}
