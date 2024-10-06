#pragma once

#include "Session.h"

//class GSClientSession : public SCSession
class GSClientSession : public SCPacketSession
{
public:
	~GSClientSession()
	{
		cout << "~GSClientSession()" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	//virtual int32 OnRecv(BYTE* InBuffer, int32 InLength) override;
	virtual void OnRecvPacket(BYTE* InBuffer, int32 InBufferLength) override;
	virtual void OnSend(int32 InLength) override;
};
