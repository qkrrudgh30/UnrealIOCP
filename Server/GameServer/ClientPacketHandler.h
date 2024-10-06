#pragma once

enum class EPacketType : uint16
{
	S_TEST = 1
};

struct GSVector
{
	union
	{
		float Data[3];

		struct
		{
			float LocationX;
			float LocationY;
			float LocationZ;
		};

		struct
		{
			float RotationX;
			float RotationY;
			float RotationZ;
		};

		struct
		{
			float ScaleX;
			float ScaleY;
			float ScaleZ;
		};
	};

	GSVector(float InX, float InY, float InZ)	
		: LocationX(InX)
		, LocationY(InY)
		, LocationZ(InZ)
	{

	}

	float& operator[](int InIndex)
	{
		return Data[InIndex];
	}
};

struct GSObjectInfo
{
	uint64 ObjectID;

	std::vector<GSVector> Transform;

};

class GSClientPacketHandler
{
public:
	static void HandlePacket(BYTE* InReceivedBuffer, int32 InReceivedBufferLength);

	static SharedPtrSCSendBuffer Make_S_TEST(uint64 InID, std::vector<GSVector> InTransform);

};
