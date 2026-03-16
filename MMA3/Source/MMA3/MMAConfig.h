// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
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

	UFUNCTION(BlueprintCallable)
	static UMMAConfig* LoadConfig() {
		bool l_SaveGameExists = UGameplayStatics::DoesSaveGameExist(MMA_SAVE_GAME_SLOT_NAME, 0);
		if (l_SaveGameExists) {
			Instance = Cast<UMMAConfig>(UGameplayStatics::LoadGameFromSlot(MMA_SAVE_GAME_SLOT_NAME, 0));
		}
		else {
			Instance = Cast<UMMAConfig>(UGameplayStatics::CreateSaveGameObject(UMMAConfig::StaticClass()));
			UGameplayStatics::SaveGameToSlot(Instance, MMA_SAVE_GAME_SLOT_NAME, 0);
		}
		return Instance;
	}

	UFUNCTION(BlueprintCallable)
	static void SaveConfig(UMMAConfig* x) {
		UGameplayStatics::SaveGameToSlot(x, MMA_SAVE_GAME_SLOT_NAME, 0);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GamePath = FString("C:\\Users\\lphel\\BSManager\\BSInstances\\1.40.3");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LeftEditorColor = FLinearColor(0.8f, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor RightEditorColor = FLinearColor(0, 0.5f, 0.8f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MappingGridScale = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SoundVolume = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitSoundVolume = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SoundVisDetail = 64;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LogarithmicFrequency = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SoundVisVerticesX = 192;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SoundVisVerticesY = 64;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SoundVisAttenuation = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> MappingDivisions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AllowResets = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AllowWristRolls = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AllowHardWristRolls = false;
};

UMMAConfig* UMMAConfig::Instance = nullptr;