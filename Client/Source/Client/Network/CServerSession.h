// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FSocket;
class UCRecvThread;

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

	void HandleRecvPackets();

	void Disconnect();

public:
	FSocket* Socket;

	TSharedPtr<UCRecvThread> RecvThread;

	TQueue<TArray<uint8>> RecvPacketQueue;
		// GameThread와 NetworkThread가 데이터를 주고 받는 공용 큐.
		// std::queue는 thread-safe하지 않음. 굳이 쓰겠다면 lock을 걸고 써야함. TQueue<>는 lock-free 방식으로 구현되어서 thread-safe함.
	
};
