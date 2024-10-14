// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Client.h"

enum class EPacketType : uint16
{
	S_TEST = 0000,
};

struct FObjectInfo
{
	uint64 ObjectID;
};

/**
 * 
 */
class CLIENT_API CServerPacketHandler 
{
public:
	static void HandlePacket(SharedPtrCServerSession& InServerSession, BYTE* InBuffer, int32 InBufferSize);

	static void Handle_S_TEST(SharedPtrCServerSession& InServerSession, BYTE* InBuffer, int32 InBufferSize);
	
};
