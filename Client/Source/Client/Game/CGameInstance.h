// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	virtual void Shutdown() override;

protected:
	void ConnectToServer();

	void DisconnectFromServer();

public:
	FSocket* ClientSocket = nullptr;
	
	FString ServerIPAddress = TEXT("127.0.0.1");

	int16 ServerPortNumber = 8080;
	
};
