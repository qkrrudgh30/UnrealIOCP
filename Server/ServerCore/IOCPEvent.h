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

class SCIOCPEvent : public OVERLAPPED // ����� ���ؼ� Overlapped �ڷ��������� �����ο� ��ĳ���� ����.
{
public:
	SCIOCPEvent(EEventType InEventType);

	// virtual ~SIOCPEvent();
	// �Ҹ��ڴ� virtual Ű���带 ��������. SCIOCPEvent�� ��� �ްԲ� ������ ��� �ֱ� ������, ��� ������� �Ҹ�ǰԲ� �ϱ� ����.
	// �׷��� virtual Ű���带 ������ SCIOCPEvent ��ü�� �� �պκп� �����Լ� ���̺� �����Ͱ� ��ġ�ϰ� ��. ��, �޸� ���̾ƿ��� ������ �ʷ�.
	// OVERLAPPED��� C ����ü�� Ư�� �޸� ���̾ƿ��� ������ �̰� �����Ǿ� �־�� Windows API�� �ùٸ��� �۵���. 
	// ���� �Ҹ��ڸ� �����ϸ� �ȵ�.

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
	std::vector<BYTE> SendBuffer;
		// �ӽ� �ڵ�
};

class SCDisconnectEvent : public SCIOCPEvent
{
public:
	SCDisconnectEvent()
		: SCIOCPEvent(EEventType::Disconnect)
	{
	}
};
