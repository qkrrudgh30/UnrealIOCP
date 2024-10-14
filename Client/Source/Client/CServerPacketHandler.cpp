// Fill out your copyright notice in the Description page of Project Settings.


#include "CServerPacketHandler.h"

#include "CNetworkThread.h"
#include "CReadableBuffer.h"
#include "CServerSession.h"
#include "CWritableBuffer.h"


void CServerPacketHandler::HandlePacket(SharedPtrCServerSession& InServerSession, BYTE* InBuffer, int32 InBufferSize)
{
	CReadableBuffer ReadableBuffer(InBuffer, InBufferSize);

	FPacketHeader PacketHeader;
	ReadableBuffer >> PacketHeader;

	switch (PacketHeader.PacketID)
	{
	case EPacketType::S_TEST:
		Handle_S_TEST(InServerSession, InBuffer, InBufferSize);
		break;
	default:
		break;
	}
}

void CServerPacketHandler::Handle_S_TEST(SharedPtrCServerSession& InServerSession, BYTE* InBuffer, int32 InBufferSize)
{
	CReadableBuffer ReadableBuffer(InBuffer, InBufferSize);

	FPacketHeader PacketHeader;
	ReadableBuffer >> PacketHeader;

	uint64 ObjectID;
	ReadableBuffer >> ObjectID;

	FString StringToPrint = FString::Printf(TEXT("ObjectID: %lld"), ObjectID);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, StringToPrint);

	char DataToSend[100] = "Hi, Server~";
	uint16 DataSize = static_cast<uint16>(std::strlen(DataToSend) + 1);
	uint16 PacketSize = DataSize + sizeof(FPacketHeader);

	SharedPtrCSendBuffer SendBuffer = MakeShared<CSendBuffer>(PacketSize);
	
	CWritableBuffer WritableBuffer(SendBuffer->GetBuffer(), PacketSize);
	WritableBuffer << PacketSize;
	WritableBuffer << static_cast<uint16>(EPacketType::S_TEST);
	WritableBuffer.Write(DataToSend, DataSize);
	SendBuffer->Close(PacketSize);
	
	InServerSession->SendPacket(SendBuffer);
}
