#pragma once

class SCSendBuffer : public std::enable_shared_from_this<SCSendBuffer>
{
public:
	SCSendBuffer();
	SCSendBuffer(int32 InCapacity);
	~SCSendBuffer();

	BYTE* GetBuffer() { return Buffer.data(); }
	int32 GetCount() { return Count; }
	int32 GetCapacity() { return static_cast<int32>(Buffer.size()); }

	void CopyData(void* InData, int32 InLength);

private:
	int32 Count = 0;

	std::vector<BYTE> Buffer;

};

