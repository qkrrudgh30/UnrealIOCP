#include "pch.h"
#include "ClientPacketHandler.h"
#include "ClientSessionManager.h"

void GSClientPacketHandler::HandlePacket(BYTE* InReceivedBuffer, int32 InReceivedBufferSize)
{
	SCReadableBuffer ReadableBuffer(InReceivedBuffer, InReceivedBufferSize);

	char DataToRecv[100] = { 0, };
	SCPacketHeader PacketHeader;
	ReadableBuffer.Read(&PacketHeader);
	ReadableBuffer.Read(DataToRecv);
	//ReadableBuffer.Peek(&PacketHeader);
	//ReadableBuffer.Peek(&PacketHeader + PacketHeader.PacketSize);

	cout << DataToRecv << endl;
}

SharedPtrSCSendBuffer GSClientPacketHandler::Make_S_TEST(uint64 InID)
{
	char DataToSend[1000] = "Hello, Client!";
	SharedPtrSCSendBuffer BufferToSend = make_shared<SCSendBuffer>(4096);
	BufferToSend->CopyData(DataToSend, 1000);

	SCWritableBuffer WritableBuffer(BufferToSend->GetBuffer(), BufferToSend->GetCapacity());

	SCPacketHeader* PacketHeader = WritableBuffer.Reserve<SCPacketHeader>();
	WritableBuffer << InID;

	PacketHeader->PacketSize = WritableBuffer.GetCurrentWriteIndex();
	PacketHeader->PacketID = (uint16)(EPacketType::S_TEST);

	return BufferToSend;
}
