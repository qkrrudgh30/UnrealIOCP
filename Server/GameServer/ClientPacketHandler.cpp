#include "pch.h"
#include "ClientPacketHandler.h"

void GSClientPacketHandler::HandlePacket(BYTE* InReceivedBuffer, int32 InReceivedBufferLength)
{
	SCReadableBuffer ReadableBuffer(InReceivedBuffer, InReceivedBufferLength);

	SCPacketHeader PacketHeader;
	ReadableBuffer.Peek(&PacketHeader);
}

SharedPtrSCSendBuffer GSClientPacketHandler::Make_S_TEST(uint64 InID, std::vector<GSVector> InTransform)
{
	SharedPtrSCSendBuffer BufferToSend = make_shared<SCSendBuffer>();

	SCWritableBuffer WritableBuffer(BufferToSend->GetBuffer(), BufferToSend->GetCapacity());

	SCPacketHeader* PacketHeader = WritableBuffer.Reserve<SCPacketHeader>();
	WritableBuffer << InID;

	WritableBuffer << (uint16)(InTransform.size());
	for (GSVector& Vector : InTransform)
	{
		WritableBuffer << Vector.Data[0] << Vector.Data[1] << Vector.Data[2];
	}

	PacketHeader->PacketSize = WritableBuffer.GetCurrentWriteIndex();
	PacketHeader->PacketID = (uint16)(EPacketType::S_TEST);

	return BufferToSend;
}
