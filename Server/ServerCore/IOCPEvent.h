#pragma once

class SCSession;

enum class EEventType : uint8
{
	None,
	Connect,
	Accept,
	Recv,
	Send,
	Disconnect
};

class SCIOCPEvent : public OVERLAPPED
{
public:
	SCIOCPEvent(EEventType InEventType);

	void Init();

public:
	EEventType EventType = EEventType::None;

	SharedPtrSCIOCPObject OwnerIOCPObject;

};

class SCConnectEvent : public SCIOCPEvent
{
public:
	SCConnectEvent()
		: SCIOCPEvent(EEventType::Connect)
	{
	}
};

class SCAcceptEvent : public SCIOCPEvent
{
public:
	SCAcceptEvent()
		: SCIOCPEvent(EEventType::Accept)
	{
	}

public:
	SharedPtrSCSession ClientSession = nullptr;

};

class SCRecvEvent : public SCIOCPEvent
{
public:
	SCRecvEvent() 
		: SCIOCPEvent(EEventType::Recv) 
	{
	}
};

class SCSendEvent : public SCIOCPEvent
{
public:
	SCSendEvent() 
		: SCIOCPEvent(EEventType::Send) 
	{
	}

public:
	//std::vector<BYTE> SendBuffer;
	std::vector<SharedPtrSCSendBuffer> SendBuffers;

};

class SCDisconnectEvent : public SCIOCPEvent
{
public:
	SCDisconnectEvent()
		: SCIOCPEvent(EEventType::Disconnect)
	{
	}
};
