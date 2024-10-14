#pragma once

#pragma once

enum class EPacketType : uint16
{
	S_TEST = 1,
};

struct FObjectInfo
{
	uint64 ObjectID;
};

class GSClientPacketHandler
{
public:
	static void HandlePacket(BYTE* InReceivedBuffer, int32 InReceivedBufferSize);

	static SharedPtrSCSendBuffer Make_S_TEST(uint64 InID);

};
