// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Queue.h"

class FSocket;
class CServerSession;

struct CLIENT_API FPacketHeader
{
	FPacketHeader()
		: PacketSize(0)
		, PacketID(0)
	{
	}

	FPacketHeader(uint16 InPacketSize, uint16 InPacketID)
		: PacketSize(InPacketSize)
		, PacketID(InPacketID)
	{
	}

	friend FArchive& operator<<(FArchive& Ar, FPacketHeader& Header)
	{
		Ar << Header.PacketSize;
		Ar << Header.PacketID;
		return Ar;
	}

	uint16 PacketSize; // 멤버의 순서도 아주 중요함.
	uint16 PacketID;
};

class CLIENT_API CSendBuffer : public TSharedFromThis<CSendBuffer>
{
public:
	CSendBuffer(int32 InCapacity);
	~CSendBuffer();

	BYTE* GetBuffer() { return Buffer.GetData(); }
	uint32 GetCount() { return Count; }
	uint32 GetCapacity() { return static_cast<int32>(Buffer.Num()); }

	void CopyData(void* InData, int32 InLength);
	void Close(uint32 InWriteSize);

private:
	TArray<BYTE> Buffer;
	
	uint32 Count = 0;

};

/**
 * 
 */
// Recv 패킷을 조립하는 스레드.
class CLIENT_API CRecvThread : public FRunnable
{
public:
	CRecvThread(FSocket* InSocket, TSharedPtr<CServerSession> InServerSession);
	~CRecvThread();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy();
	
private:
	bool ReceivePacket(TArray<uint8>& OutPacket);
	bool CheckReceivedPacketSize(uint8* Results, int32 Size); // 내가 원하는 크기의 패킷이 올때까지(패킷 크기만큼 다 받아질때까지)기다리는 함수. 다 받으면 Results에 담아줌.

protected:
	FRunnableThread* RecvThread = nullptr;
	bool bIsRunning = true; // 밖(GameInstance)에서 Running을 false로 꺼버리면 해당 스레드도 종료되는 방식.
	FSocket* Socket;
	TWeakPtr<CServerSession> ServerSession;
	
};
