#include "pch.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"

void GSClientSession::OnConnected()
{
	GSClientSessionManager::GetInstance().Add(static_pointer_cast<GSClientSession>(shared_from_this()));
}

void GSClientSession::OnDisconnected()
{
	GSClientSessionManager::GetInstance().Remove(static_pointer_cast<GSClientSession>(shared_from_this()));
}

int32 GSClientSession::OnRecv(BYTE* InBuffer, int32 InLength)
{
	// Echo
	cout << "OnRecv Len = " << InLength << endl;

	SharedPtrSCSendBuffer SendBuffer = make_shared<SCSendBuffer>(4096);
	SendBuffer->CopyData(InBuffer, InLength);

	GSClientSessionManager::GetInstance().Broadcast(SendBuffer);

	return InLength;
}

void GSClientSession::OnSend(int32 InLength)
{
	cout << "OnSend Len = " << InLength << endl;
}
