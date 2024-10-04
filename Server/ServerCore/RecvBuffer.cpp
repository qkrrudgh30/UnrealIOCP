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
		// �� ��ħ �б�+���� Ŀ���� ������ ��ġ���, �� �� ����.
		CurrentReadIndex = CurrentWriteIndex = 0;
	}
	else
	{
		// ���� ������ ���� 1�� ũ�� �̸��̸�, �����͸� ������ �����.
		if (GetSlot() < UnitSize)
		{
			::memcpy(&Buffer[0], &Buffer[CurrentReadIndex], CountToRead);
			CurrentReadIndex = 0;
			CurrentWriteIndex = CountToRead;
		}
	}
}
