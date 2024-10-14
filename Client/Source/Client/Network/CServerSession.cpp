// Fill out your copyright notice in the Description page of Project Settings.


#include "CServerSession.h"

#include "Sockets.h"
#include "CServerPacketHandler.h"


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

		SharedPtrCServerSession ThisPtr = AsShared();
		CServerPacketHandler::HandlePacket(ThisPtr, Packet.GetData(), Packet.Num());
		// HandlePacket() 함수는 PacketSessionRef& 자료형을 인자로 받고 있음. 왜 굳이 & 자료형일까? 참조 카운팅이 안올라가게 하려고. 카운트가 증가하는 것 조차 부하가 좀 걸릴 수 있음. Atomic 연산이라서.
		// 참조자료형이라 변수를 따로 ThisPtr로 만든것.
	}
}

void CServerSession::SendPacket(SharedPtrCSendBuffer InSendBuffer)
{
	SendPacketQueue.Enqueue(InSendBuffer);
}