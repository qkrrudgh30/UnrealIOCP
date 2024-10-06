// Fill out your copyright notice in the Description page of Project Settings.


#include "CServerPacketHandler.h"

#include "CNetworkThread.h"
#include "CReadableBuffer.h"


void CServerPacketHandler::HandlePacket(BYTE* InReceivedBuffer, int32 InReceivedBufferLength)
{
	CReadableBuffer ReadableBuffer(InReceivedBuffer, InReceivedBufferLength);

	FPacketHeader PacketHeader;
	ReadableBuffer >> PacketHeader;

	switch (PacketHeader.PacketID)
	{
	case EPacketType::S_TEST:
		Handle_S_TEST(InReceivedBuffer, InReceivedBufferLength);
		break;
	default:
		break;
	}
}

void CServerPacketHandler::Handle_S_TEST(BYTE* InReceivedBuffer, int32 InReceivedBufferLength)
{
	CReadableBuffer ReadableBuffer(InReceivedBuffer, InReceivedBufferLength);

	FPacketHeader PacketHeader;
	ReadableBuffer >> PacketHeader;

	uint64 ObjectID;
	ReadableBuffer >> ObjectID;

	FString StringToPrint = FString::Printf(TEXT("ObjectID: %lld"), ObjectID);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, StringToPrint);

	TArray<GSVector> Transform;
	uint16 Count;
	ReadableBuffer >> Count;

	for (int32 i = 0; i < Count; i++)
	{
		ReadableBuffer >> Transform[i].Data[0] >> Transform[i].Data[1] >> Transform[i].Data[2];
		
		StringToPrint = FString::Printf(TEXT("Transform[%d] (%.1f, %.1f, %.1f)"), Transform[i].Data[0], Transform[i].Data[1], Transform[i].Data[2]);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, StringToPrint);
	}
}
