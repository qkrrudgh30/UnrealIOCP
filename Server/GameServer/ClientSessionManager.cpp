#include "pch.h"
#include "ClientSessionManager.h"
#include "ClientSession.h"

GSClientSessionManager& GSClientSessionManager::GetInstance()
{
	static GSClientSessionManager Instance;

	return Instance;
}

void GSClientSessionManager::Add(SharedPtrGSClientSession InClientSession)
{
	WRITE_LOCK;
	ClientSessions.insert(InClientSession);
}

void GSClientSessionManager::Remove(SharedPtrGSClientSession InClientSession)
{
	WRITE_LOCK;
	ClientSessions.erase(InClientSession);
}

void GSClientSessionManager::Broadcast(SharedPtrSCSendBuffer InSendBuffer)
{
	WRITE_LOCK;
	for (SharedPtrGSClientSession Session : ClientSessions)
	{
		Session->Send(InSendBuffer);
	}
}
