// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameInstance.h"

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Kismet/KismetSystemLibrary.h"

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

	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Connecting to server...")), true, false, FLinearColor::Red, 60.f);

	bool bIsConnected = ClientSocket->Connect(*AddressToConnect);
	if (bIsConnected == true)
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Connection Success!")), true, false, FLinearColor::Red, 60.f);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Connection Failed...")), true, false, FLinearColor::Red, 60.f);
	}
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
