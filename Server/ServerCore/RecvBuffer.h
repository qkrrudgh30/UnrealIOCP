#pragma once

class SCRecvBuffer
{
	enum { BUFFER_COUNT = 10 };

public:
	SCRecvBuffer(int32 InUnitSize);
	~SCRecvBuffer();

	bool OnRead(int32 InBytesRead);
	bool OnWrite(int32 InBytesWrite);
	void Clean();

	BYTE* GetCurrentReadPosition() { return &Buffer[CurrentReadIndex]; }
	BYTE* GetCurrentWritePosition() { return &Buffer[CurrentWriteIndex]; }
	int32 GetCountToRead() { return CurrentWriteIndex - CurrentReadIndex; }
	int32 GetSlot() { return Capacity - CurrentWriteIndex; }

private:
	int32 Capacity = 0;

	int32 UnitSize = 0;

	int32 CurrentReadIndex = 0;

	int32 CurrentWriteIndex = 0;

	std::vector<BYTE> Buffer;

};

