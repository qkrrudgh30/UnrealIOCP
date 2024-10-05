// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameInstance.h"

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CServerSession.h"

void UCGameInstance::Init()
{
	Super::Init();

	ConnectToServer();
}

void UCGameInstance::Shutdown()
{
	Super::Shutdown();

	DisconnectFromServer();
}

void UCGameInstance::ConnectToServer()
{
	ClientSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("ClientSocket"));

	FIPv4Address IPAddressToConnect;
	FIPv4Address::Parse(ServerIPAddress, IPAddressToConnect);

	TSharedRef<FInternetAddr> AddressToConnect = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	AddressToConnect->SetIp(IPAddressToConnect.Value);
	AddressToConnect->SetPort(ServerPortNumber);

	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Connecting to server...")), true, true, FLinearColor::Red, 60.f);

	bool bIsConnected = ClientSocket->Connect(*AddressToConnect);
	if (bIsConnected == true)
	{
		ServerSession = MakeShared<CServerSession>(ClientSocket);
		ServerSession->Run();
		
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Connection Success!")), true, true, FLinearColor::Red, 60.f);

		while (true)
		{
			char SendBuffer[100] = "Hello, Server!";
			int32 SendBufferLength = 0;

			bool bDidSend = ClientSocket->Send((uint8*)SendBuffer, sizeof(SendBuffer), SendBufferLength);
			if (bDidSend == false)
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Send Failed...")), true, true, FLinearColor::Red, 60.f);
				break;
			}

			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Send Data Length: %d"), SendBufferLength), true, true, FLinearColor::Red, 60.f);

			char RecvBuffer[100] = { 0, };
			int32 RecvBufferLength = 0;
			
			bool bDidRecv = ClientSocket->Recv((uint8*)RecvBuffer, sizeof(RecvBuffer), RecvBufferLength);
			if (bDidRecv == false)
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Recv Failed...")), true, true, FLinearColor::Red, 60.f);
				break;
			}

			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Recv Data: %s"), *FString(RecvBuffer)), true, true, FLinearColor::Red, 60.f);
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Recv Data Length: %d"), RecvBufferLength), true, true, FLinearColor::Red, 60.f);
			
			FPlatformProcess::Sleep(1.0f);
		}
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Connection Failed...")), true, true, FLinearColor::Red, 60.f);
	}
}

void UCGameInstance::HandleRecvPackets()
{
	if (ClientSocket == nullptr || ServerSession == nullptr)
		return;

	ServerSession->HandleRecvPackets();
}

void UCGameInstance::DisconnectFromServer()
{
	if (ClientSocket != nullptr)
	{
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(ClientSocket);
		ClientSocket = nullptr;
	}
}
