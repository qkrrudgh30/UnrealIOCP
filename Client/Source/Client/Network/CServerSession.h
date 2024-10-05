// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Client.h"
#include "CNetworkThread.h"

class FSocket;

/**
 * 
 */
class CLIENT_API CServerSession : public TSharedFromThis<CServerSession>
{
public:
	CServerSession(FSocket* InSocket);
	~CServerSession();

public:
	void Run();

	void Disconnect();

	void HandleRecvPackets();

	void SendPacket(SharedPtrCSendBuffer InSendBuffer);

public:
	FSocket* Socket;

	TSharedPtr<CRecvThread> RecvThread;

	TQueue<TArray<uint8>> RecvPacketQueue;

	TSharedPtr<CSendThread> SendThread;

	TQueue<SharedPtrCSendBuffer> SendPacketQueue;
	
};
