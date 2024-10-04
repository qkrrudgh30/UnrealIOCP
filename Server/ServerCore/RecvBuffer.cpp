#include "pch.h"
#include "RecvBuffer.h"

SCRecvBuffer::SCRecvBuffer(int32 InUnitSize)
	: UnitSize(InUnitSize)
{
	Capacity = UnitSize * BUFFER_COUNT;
	Buffer.resize(Capacity);
}

SCRecvBuffer::~SCRecvBuffer()
{
}

bool SCRecvBuffer::OnRead(int32 InBytesRead)
{
	if (GetCountToRead() < InBytesRead)
	{
		return false;
	}

	CurrentReadIndex += InBytesRead;

    return true;
}

bool SCRecvBuffer::OnWrite(int32 InBytesWrite)
{
	if (GetSlot() < InBytesWrite)
	{
		return false;
	}

	CurrentWriteIndex += InBytesWrite;

    return true;
}

void SCRecvBuffer::Clean()
{
	int32 CountToRead = GetCountToRead();
	if (CountToRead == 0)
	{
		// 딱 마침 읽기+쓰기 커서가 동일한 위치라면, 둘 다 리셋.
		CurrentReadIndex = CurrentWriteIndex = 0;
	}
	else
	{
		// 여유 공간이 버퍼 1개 크기 미만이면, 데이터를 앞으로 땅긴다.
		if (GetSlot() < UnitSize)
		{
			::memcpy(&Buffer[0], &Buffer[CurrentReadIndex], CountToRead);
			CurrentReadIndex = 0;
			CurrentWriteIndex = CountToRead;
		}
	}
}
