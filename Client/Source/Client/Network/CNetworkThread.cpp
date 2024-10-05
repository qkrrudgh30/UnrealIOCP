// Fill out your copyright notice in the Description page of Project Settings.


#include "CNetworkThread.h"
#include "Sockets.h"
#include "CServerSession.h"

CSendBuffer::CSendBuffer(int32 InCapacity)
{
	Buffer.SetNum(InCapacity);
}

CSendBuffer::~CSendBuffer()
{
}

void CSendBuffer::CopyData(void* InData, int32 InLength)
{
	::memcpy(Buffer.GetData(), InData, InLength);
	Count = InLength;
}

void CSendBuffer::Close(uint32 InWriteSize)
{
	Count = InWriteSize;
}

CRecvThread::CRecvThread(FSocket* InSocket, TSharedPtr<CServerSession> InServerSession)
	: Socket(InSocket)
	, ServerSession(InServerSession)
{
	RecvThread = FRunnableThread::Create(this, TEXT("RecvThread"));
}

CRecvThread::~CRecvThread()
{
}

bool CRecvThread::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Initialize RecvThread.")));
	
	return FRunnable::Init();
}

uint32 CRecvThread::Run()
{
	while (bIsRunning)
	{
		TArray<uint8> Packet;

		if (ReceivePacket(OUT Packet))
		{
			// recv 패킷 하나를 완성했다면 프로토버프를 통해 만들어진 패킷 개체로 다시 변환해줘야함.
			// 그래야 자동완성되는 HandlePacket() 함수의 인자로 해당 recv 패킷을 전달 할 수 있음.
			// 근데 이걸 지금 하기엔 너무 이름. 자식 스레드(RecvWorker)에서 만든 데이터를 곧바로 메인 스레드에서 접근 하게끔 만드면 크래시 내버림.
			// 그래서 어딘가에 넣어둬야함. 메인 스레드가 나중에 꺼내서 사용할 수 있게끔.
			// 우리는 큐를 만들고자 함. 그럼 또 그 큐를 어디에 둘지가 고민. GameInstance도 좋고, Session에도 좋고.. 우리는 PacketSession에 두고자 함.
			if (TSharedPtr<CServerSession> Session = ServerSession.Pin()) // Pin() 함수는 C++ 표준의 shared_ptr::lock() 함수와 동일한 역할.
			{ // 만약  Session을 가져오는 것 까지는 좋았는데, Enqueue() 하는 순간에 Session이 죽어버린다면? 그럴일은 없음. TSharedPtr이기 때문에 이미 가져올 때부터 참조카운팅이 늘어남.
				Session->RecvPacketQueue.Enqueue(Packet);
			}
		}
		
		// Q. 이 위치에 Sleep() 함수는 걸어주지 않아도 될까? recv() 함수가 블로킹 방식이라 데이터가 없으면 알아서 흐름이 멈춰버림. 즉, ReceiveDesiredBytes() 함수에서 Recv() 함수를 호출하는 순간에 알아서 잠들기 때문에 Sleep() 호출할 필요 없음.
	}

	return 0;
}

void CRecvThread::Exit()
{
	FRunnable::Exit();
}

void CRecvThread::Destroy()
{
	bIsRunning = false;
}

bool CRecvThread::ReceivePacket(TArray<uint8>& OutPacket)
{
	// 패킷 헤더 파싱
	const int32 HeaderSize = sizeof(FPacketHeader); 
	TArray<uint8> HeaderBuffer;
	HeaderBuffer.AddZeroed(HeaderSize); // AddZeroed() 함수는 reserve() 함수와 유사한 역할. 공간 확보.

	if (CheckReceivedPacketSize(HeaderBuffer.GetData(), HeaderSize) == false) // 헤더 크기만큼은 받았는지 꼭 체크 해야함.
		return false;

	// ID, Size 파싱
	FPacketHeader Header;
	{
		FMemoryReader Reader(HeaderBuffer);
		Reader << Header;
		UE_LOG(LogTemp, Log, TEXT("Recv PacketID : %d, PacketSize : %d"), Header.PacketID, Header.PacketSize);
	}

	// 패킷 헤더 복사
	OutPacket = HeaderBuffer;

	// 패킷 내용 파싱
	TArray<uint8> PayloadBuffer;
	const int32 PayloadSize = Header.PacketSize - HeaderSize;
	if (PayloadSize == 0) // 이 예외처리 구문이 없다면 AddZeroed()에서 크래시남. 0만큼 공간 확보하겠다는 거니까. 가끔 패킷 중에는 내용물이 없을 수 있음 헤더만 있는 경우. 그럴때 문제됨.
		return true;
	
	OutPacket.AddZeroed(PayloadSize);

	if (CheckReceivedPacketSize(&OutPacket[HeaderSize], PayloadSize))
		return true;

	return false;
}

bool CRecvThread::CheckReceivedPacketSize(uint8* Results, int32 Size)
{
	uint32 PendingDataSize = 0;
	// if (Socket == nullptr || Socket->GetConnectionState() != ESocketConnectionState::SCS_Connected) // 소켓이 없거나 연결이 끊겼을때
	// {
	// 	Socket = nullptr;
	// 	return false;
	// }
	
	if (Socket->HasPendingData(PendingDataSize) == false || PendingDataSize <= 0) // 접속 종료시
		return false;

	int32 Offset = 0;

	while (Size > 0) // Size: 내가 받아야하는 패킷의 크기.
	{
		int32 NumRead = 0;
		Socket->Recv(Results + Offset, Size, OUT NumRead); // NumRead: 이번에 읽어들인 크기.
		check(NumRead <= Size);

		if (NumRead <= 0) // NumRead가 0이란소리는 종료되었단 소리.
			return false;

		Offset += NumRead;
		Size -= NumRead;
	}

	return true;
}

CSendThread::CSendThread(FSocket* InSocket, TSharedPtr<CServerSession> InSession)
	: Socket(InSocket)
	, ServerSession(InSession)
{
	SendThread = FRunnableThread::Create(this, TEXT("SendThread"));
}

CSendThread::~CSendThread()
{
}

bool CSendThread::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Initialize SendThread")));

	return FRunnable::Init();
}

uint32 CSendThread::Run()
{
	while (bIsRunning)
	{
		SharedPtrCSendBuffer SendBuffer;

		if (TSharedPtr<CServerSession> Session = ServerSession.Pin()) // Recv 패킷때와 반대. 
		{
			if (Session->SendPacketQueue.Dequeue(OUT SendBuffer))
			{
				SendPacket(SendBuffer);
			}
		}

		//@TODO: Sleep() 함수 호출.
	}

	return 0;
}

void CSendThread::Exit()
{
	FRunnable::Exit();
}

bool CSendThread::SendPacket(SharedPtrCSendBuffer InSendBuffer)
{
	if (CheckSentPacketSize(InSendBuffer->GetBuffer(), InSendBuffer->GetCount()) == false) // 조립이 이미 다 되어있을테니까 일단 보내기만 함.
		return false;

	return true;
}

void CSendThread::Destroy()
{
	bIsRunning = false;
}

bool CSendThread::CheckSentPacketSize(const uint8* InBuffer, int32 InSize)
{
	while (0 < InSize)
	{
		int32 BytesSent = 0;
		if (Socket->Send(InBuffer, InSize, BytesSent) == false)
			return false;

		InSize -= BytesSent;
		InBuffer += BytesSent;
	}

	return true;
}
