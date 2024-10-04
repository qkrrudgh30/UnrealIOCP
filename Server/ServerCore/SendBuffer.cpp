#include "pch.h"
#include "SendBuffer.h"

SCSendBuffer::SCSendBuffer()
{
	Buffer.resize(4096);
}

SCSendBuffer::SCSendBuffer(int32 InCapacity)
{
	Buffer.resize(InCapacity);
}

SCSendBuffer::~SCSendBuffer()
{
}

void SCSendBuffer::CopyData(void* InData, int32 InLength)
{
	assert(InLength <= Buffer.size() && "Buffer.size() < InLength");
	::memcpy(Buffer.data(), InData, InLength);
	Count = InLength;
}
