// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

#define CheckObj(var) \
if (var == nullptr) {\
	UE_LOG(LogScript, Error, TEXT("Pointer \"%s\" is undefined: %p"), #var, var);\
	return;\
}