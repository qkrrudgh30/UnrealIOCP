// Fill out your copyright notice in the Description page of Project Settings.

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

	TArray<GSVector> Transform;
	
};

/**
 * 
 */
class CLIENT_API CServerPacketHandler
{
public:
	static void HandlePacket(BYTE* InReceivedBuffer, int32 InReceivedBufferLength);

	static void Handle_S_TEST(BYTE* InReceivedBuffer, int32 InReceivedBufferLength);

};
