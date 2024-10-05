// Fill out your copyright notice in the Description page of Project Settings.


#include "CServerSession.h"

#include "Sockets.h"


CServerSession::CServerSession(FSocket* InSocket)
	: Socket(InSocket)
{
}

CServerSession::~CServerSession()
{
	Disconnect();
}

void CServerSession::Run()
{
	RecvThread = MakeShared<CRecvThread>(Socket, AsShared());
	SendThread = MakeShared<CSendThread>(Socket, AsShared());
}

void CServerSession::Disconnect()
{
	if (RecvThread)
	{
		RecvThread->Destroy();
		RecvThread = nullptr;
	}

	if (SendThread)
	{
		SendThread->Destroy();
		SendThread = nullptr;
	}
}

void CServerSession::HandleRecvPackets()
{
	while (true)
	{
		TArray<uint8> Packet;
		if (RecvPacketQueue.Dequeue(OUT Packet) == false)
			break;

		// 여기부터는 recv 패킷을 꺼내온 상태.
		// 해당 패킷을 처리할때는 패킷의 번호를 보고, 번호에 맞는 함수를 호출하면서 해당 패킷을 인자로 전달해주는 식으로 구현함.
		// 조금 아쉬운 프로젝트에서는 이를 switch-case 문으로 처리함. case가 몇백개씩 늘어남.
		// 우리는 나중에 ClientPacketHandler 클래스에서 이를 처리할 예정.
	}
}

void CServerSession::SendPacket(SharedPtrCSendBuffer InSendBuffer)
{
	SendPacketQueue.Enqueue(InSendBuffer);
}