// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MMAConfig.generated.h"

#define MMA_SAVE_GAME_SLOT_NAME FString("MMA3Config")

/**
 *
 */
UCLASS()
class MMA3_API UMMAConfig : public USaveGame
{
	GENERATED_BODY()

public:

	UMMAConfig();

	static UMMAConfig* Instance;

	UPROPERTY(EditAnywhere)
		FString GamePath = FString("");

};

UMMAConfig* UMMAConfig::Instance = nullptr;