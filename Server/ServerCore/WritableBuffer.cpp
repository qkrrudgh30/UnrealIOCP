#include "pch.h"
#include "WritableBuffer.h"

SCWritableBuffer::SCWritableBuffer()
{
}

SCWritableBuffer::SCWritableBuffer(BYTE* InBuffer, uint32 InCapacity, uint32 InCurrentWriteIndex)
	: Buffer(InBuffer)
	, Capacity(InCapacity)
	, CurrentWriteIndex(InCurrentWriteIndex)
{

}

SCWritableBuffer::~SCWritableBuffer()
{
}

bool SCWritableBuffer::Write(void* InStartPtrToWrite, uint32 InWriteLength)
{
    if (GetSlot() < InWriteLength)
    {
        return false;
    }

    ::memcpy(&Buffer[CurrentWriteIndex], InStartPtrToWrite, InWriteLength);
    CurrentWriteIndex += InWriteLength;
    return true;
}
