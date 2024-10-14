#include "pch.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "ClientPacketHandler.h"

void GSClientSession::OnConnected()
{
	GSClientSessionManager::GetInstance().Add(static_pointer_cast<GSClientSession>(shared_from_this()));
}

void GSClientSession::OnDisconnected()
{
	GSClientSessionManager::GetInstance().Remove(static_pointer_cast<GSClientSession>(shared_from_this()));
}

int32 GSClientSession::OnRecvPacket(BYTE* InBuffer, int32 InBufferLength)
{
	SCPacketHeader PacketHeader = *((SCPacketHeader*)InBuffer);
	//cout << "Packet ID : " << PacketHeader.PacketID << "Size : " << PacketHeader.PacketSize << endl;

	GSClientPacketHandler::HandlePacket(InBuffer, InBufferLength);

	return InBufferLength;
}

void GSClientSession::OnSend(int32 InLength)
{
	//cout << "OnSend Len = " << InLength << endl;
}
