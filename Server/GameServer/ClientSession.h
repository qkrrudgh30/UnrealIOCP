#pragma once

#include "Session.h"

class GSClientSession : public SCSession
{
public:
	~GSClientSession()
	{
		cout << "~GSClientSession()" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual int32 OnRecv(BYTE* InBuffer, int32 InLength) override;
	virtual void OnSend(int32 InLength) override;
};
