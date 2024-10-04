#pragma once

class GSClientSession;

using SharedPtrGSClientSession = std::shared_ptr<GSClientSession>;

class GSClientSessionManager
{
public:
	static GSClientSessionManager& GetInstance();

	void Add(SharedPtrGSClientSession InClientSession);
	void Remove(SharedPtrGSClientSession InClientSession);
	void Broadcast(SharedPtrSCSendBuffer InSendBuffer);

private:
	USE_LOCK;
	std::set<SharedPtrGSClientSession> ClientSessions;

};
