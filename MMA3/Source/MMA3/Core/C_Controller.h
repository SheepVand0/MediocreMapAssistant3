// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "MMA3/Widgets/MainMenu/LevelSelectionWidget.h"
#include "MMA3/Widgets/MainMenu/MapDetailsWidget.h"
#include "Components/AudioComponent.h"
#include "BeatCell.h"
#include "MMA3/MMAConfig.h"
#include "C_Controller.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeUpdated, float, Time);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNeedToAddBeatmapsObjects, FMapData, mapContent, ABeatCell*, beatCells);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNeedToUpdateMapperPawnPosition, float, YPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNeedToResetMapperPawnTransform);

UCLASS()
class MMA3_API AC_Controller : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AC_Controller();

	UPROPERTY(EditAnywhere)
		ULevelSelectionWidget* m_MapSelectionWidget;

	UPROPERTY(EditAnywhere)
		UMapDetailsWidget* m_MapDetailsWidget;

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

	UPROPERTY()
		APlayerController* PlayerControllerReference;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* TimeMarkerCube;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MappingGrid;

	UPROPERTY()
		ABeatCell* BeatCells;

	UPROPERTY()
		UAudioComponent* AudioComponent;

	UPROPERTY()
		FString CurrentScene = "MainMenu";

	UPROPERTY()
		FMapData CurrentMapData;

	UStaticMesh* BombMesh;


	UStaticMesh* DotMesh;


	UStaticMesh* CubeMesh;


	UMaterialInterface* WallMaterial;


	UMaterialInstance* BombMaterial;

	USoundWave* HitSound;

	UPROPERTY()
		float PlayingTime;

	UPROPERTY()
		float StartedPlayTime;

	UPROPERTY()
		bool Playing;

	UPROPERTY()
		float SongDuration;

	UPROPERTY()
		FMapInfo MapData;

	UPROPERTY()
		FMapData MapContent;

	UPROPERTY()
		float ActorTime;

	UFUNCTION()
		void Play();

	UFUNCTION()
		void Stop();

	UFUNCTION()
		void GenerateGrid(FMapInfo p_Info, FString p_Diff, FString p_Mode);

	UFUNCTION()
		void AddTime(float p_Time);

	UFUNCTION()
		void UpdateBeatGrid();

	static AC_Controller* Instance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

AC_Controller* AC_Controller::Instance = nullptr;