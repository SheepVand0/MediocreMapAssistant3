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
		FString GamePath = FString("C:\\Users\\user\\Desktop\\BS1.18.3\\BSLegacyLauncher\\Installed Versions\\Beat Saber 1.26.0");

	UPROPERTY(EditAnywhere)
		FColor LeftEditorColor = FColor(0.2f, 0, 0);

	UPROPERTY(EditAnywhere)
		FColor RightEditorColor = FColor(0, 0, 0.2f);

};

UMMAConfig* UMMAConfig::Instance = nullptr;