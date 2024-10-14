// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define USING_SHARED_PTR(InClassName) using SharedPtr##InClassName = TSharedPtr<class InClassName>;

USING_SHARED_PTR(CServerSession);
USING_SHARED_PTR(CSendBuffer);

