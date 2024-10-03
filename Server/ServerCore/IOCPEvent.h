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

class SCIOCPEvent : public OVERLAPPED // 상속을 통해서 Overlapped 자료형으로의 자유로운 업캐스팅 가능.
{
public:
	SCIOCPEvent(EEventType InEventType);

	// virtual ~SIOCPEvent();
	// 소멸자는 virtual 키워드를 붙혀야함. SCIOCPEvent를 상속 받게끔 구조를 잡고 있기 때문에, 상속 순서대로 소멸되게끔 하기 위함.
	// 그런데 virtual 키워드를 붙히면 SCIOCPEvent 개체의 맨 앞부분에 가상함수 테이블 포인터가 위치하게 됨. 즉, 메모리 레이아웃의 변경을 초래.
	// OVERLAPPED라는 C 구조체는 특정 메모리 레이아웃을 가지고 이게 고정되어 있어야 Windows API가 올바르게 작동함. 
	// 따라서 소멸자를 정의하면 안됨.

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
		// 임시 코드
};

class SCDisconnectEvent : public SCIOCPEvent
{
public:
	SCDisconnectEvent()
		: SCIOCPEvent(EEventType::Disconnect)
	{
	}
};
