#include "pch.h"
#include "ReadableBuffer.h"

SCReadableBuffer::SCReadableBuffer()
{
}

SCReadableBuffer::SCReadableBuffer(BYTE* InBuffer, uint32 InCapacity, uint32 InCurrentReadIndex = 0)
	: Buffer(InBuffer)
	, Capacity(InCapacity)
	, CurrentReadIndex(InCurrentReadIndex)
{

}

SCReadableBuffer::~SCReadableBuffer()
{
}

bool SCReadableBuffer::Peek(void* InStartPtrToRead, uint32 InReadLength)
{
	if (GetSlot() < InReadLength)
		return false;

	::memcpy(InStartPtrToRead, &Buffer[CurrentReadIndex], InReadLength);
	return true;
}

bool SCReadableBuffer::Read(void* InStartPtrToRead, uint32 InReadLength)
{
	if (Peek(InStartPtrToRead, InReadLength) == false)
		return false;

	CurrentReadIndex += InReadLength;
	return true;
}
