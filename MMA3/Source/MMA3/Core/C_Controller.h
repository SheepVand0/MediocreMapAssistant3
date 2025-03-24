// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "MMA3/Widgets/MainMenu/LevelSelectionWidget.h"
#include "MMA3/Widgets/MainMenu/MapDetailsWidget.h"
#include "MMA3/Widgets/EditMode/EditModeWidget.h"
#include "Components/AudioComponent.h"
#include "BeatCell.h"
#include "MMA3/MMAConfig.h"
#include "MMA3/Core/RenderWaveForm.h"
#include "C_Controller.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeUpdated, float, Time);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNeedToAddBeatmapsObjects, FMapData, mapContent, ABeatCell*, beatCells);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNeedToUpdateMapperPawnPosition, float, YPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNeedToResetMapperPawnTransform);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNeedToCreateEditModeWidget);

UCLASS()
class MMA3_API AC_Controller : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AC_Controller();

	static AC_Controller* Instance;

protected:

	UPROPERTY(EditAnywhere)
	ULevelSelectionWidget* MapSelectionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEditModeWidget* EditModeWidget;

	UPROPERTY(BlueprintAssignable)
	FOnNeedToCreateEditModeWidget OnNeedToCreateEditModeWidget;

	UPROPERTY(EditAnywhere)
	UMapDetailsWidget* MapDetailsWidget;

	UPROPERTY()
	APlayerController* PlayerControllerReference;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TimeMarkerCube;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MappingGrid;

	UPROPERTY(EditAnywhere)
	UProceduralMeshComponent* WaveformMesh;

	UPROPERTY()
	ABeatCell* BeatCells;

	UPROPERTY()
	UAudioComponent* AudioComponent;

	UPROPERTY()
	FString CurrentScene = "MainMenu";

	UPROPERTY()
	/// <summary>
	/// In sec
	/// </summary>
	float PlayingTime;

	UPROPERTY()
	float StartedPlayTime;

	UPROPERTY()
	bool Playing;

	UPROPERTY()
	float SongDuration;

	UPROPERTY()
	FMapInfo MapInfo;

	UPROPERTY()
	FMapData MapContent;

	UPROPERTY()
	float ActorTime;

	UFUNCTION()
	void WidgetTimeSliderChanged(float newValue);

public:

	UPROPERTY(EditAnywhere)
	FOnReady OnControllerReady;

	UPROPERTY(BlueprintAssignable)
	FOnTimeUpdated OnTimeUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnNeedToAddBeatmapsObjects OnNeedToAddBeatmapObjects;

	UPROPERTY(BlueprintAssignable)
	FOnNeedToUpdateMapperPawnPosition OnNeedToUpdateMapperPawnPosition;

	UPROPERTY(BlueprintAssignable)
	FOnNeedToResetMapperPawnTransform OnNeedToResetMapperPawnTransform;

	UStaticMesh* BombMesh;

	UStaticMesh* DotMesh;

	UStaticMesh* CubeMesh;

	UMaterialInterface* WallMaterial;

	UMaterialInterface* GlobalNoteMaterial;
	UMaterialInterface* RightNoteMaterial;
	UMaterialInterface* LeftNoteMaterial;
	UMaterialInterface* PassedRightNoteMaterial;
	UMaterialInterface* PassedLeftNoteMaterial;
	UMaterialInstance* BombMaterial;

	UMaterialInterface* WaveformMaterial;

	USoundWave* HitSound;

	UFUNCTION()
	void Play();

	UFUNCTION()
	void Stop();

	UFUNCTION()
	void GenerateGrid(FMapInfo p_Info, FString p_Diff, FString p_Mode);

	UFUNCTION()
	void AddTime(float time);

	UFUNCTION()
	void SetTime(float time);

	UFUNCTION()
	void UpdateBeatGrid();

	UFUNCTION()
	void UpdateNotesMaterial();

	UFUNCTION()
	void SetCurrentScene(FString sceneName);

//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION()
	float GetBeat();

	UFUNCTION()
	float GetPlayTime();

	UFUNCTION()
	bool IsPlaying();

	UFUNCTION()
	FMapData GetMapData();

	UFUNCTION()
	FString GetCurrentSceneName();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

AC_Controller* AC_Controller::Instance = nullptr;