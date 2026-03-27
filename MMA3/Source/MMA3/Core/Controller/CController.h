// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "MMA3/Widgets/MainMenu/LevelSelectionWidget.h"
#include "MMA3/Widgets/MainMenu/MapDetailsWidget.h"
#include "MMA3/Widgets/EditMode/EditModeWidget.h"
#include "MMA3/Widgets/MainMenu/Settings/MainSettings.h"
#include "Components/AudioComponent.h"
#include "MMA3/Core/Mapping/BeatCell.h"
#include "MMA3/MMAConfig.h"
#include "MMA3/Core/SoundVisualization/RenderWaveForm.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Materials/MaterialInstanceConstant.h"
#include "MMA3/Core/BeatmapUtils.h"
#include "MMA3/Core/CHistory.h"
#include "CController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeUpdated, float, Time);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnNeedToAddBeatmapsObjects, FMapData, mapContent, ABeatCell*, beatCells, float, beat, float, renderDistance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNeedToDestroyBeatmapObjects);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNeedToUpdateMapperPawnPosition, float, YPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNeedToResetMapperPawnTransform);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNeedToCreateWidgets);

#define SOUND_VIS_VERTICES_X 192
#define SOUND_VIS_VERTICES_Y 64

UCLASS()
class MMA3_API ACController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACController();

	static ACController* Instance;

	virtual void OnConstruction(const FTransform& transform) override;

	static FString TimeToString(float secs);

	static FString MinZeros(FString value, int count);

	UFUNCTION(BlueprintCallable)
	static ACController* GetControllerRef();
		
protected:

	UPROPERTY(BlueprintReadOnly)
	UAudioComponent* HitSoundAudioComponent;

public:

	UPROPERTY(EditAnywhere)
	ULevelSelectionWidget* LevelSelectionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEditModeWidget* EditModeWidget;

	UPROPERTY(BlueprintAssignable)
	FOnNeedToCreateWidgets OnNeedToCreateWidgets;

	UPROPERTY(EditAnywhere)
	UMapDetailsWidget* MapDetailsWidget;

	UPROPERTY(EditAnywhere)
	UMainSettings* MainSettingsWidget;

protected:

	UPROPERTY()
	APlayerController* PlayerControllerReference;

	UPROPERTY(EditAnywhere)
	USceneComponent* MappingRoot;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TimeMarkerCube;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MappingGrid;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UProceduralMeshComponent* SoundVisMesh;

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> SoundVisVertices;

	UPROPERTY(BlueprintReadOnly)
	TArray<FLinearColor> SoundVisVertexColors;

	UPROPERTY()
	TObjectPtr <ABeatCell> BeatCells;

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
	float ActorTime;

	UPROPERTY()
	float RenderDistance;

	UPROPERTY()
	float DeltaTime;

	UPROPERTY()
	float LastPlayingTime;

	UFUNCTION()
	void WidgetTimeSliderChanged(float newValue);

	
public:

	UPROPERTY()
	UAudioComponent* AudioComponent;

	//UPROPERTY()
	FMapInfo* MapInfo;

	FMapData* MapContent;

	FMapDifficulty* Difficulty;

	UPROPERTY(EditAnywhere)
	FOnReady OnControllerReady;

	UPROPERTY(BlueprintAssignable)
	FOnTimeUpdated OnTimeUpdated;

	//UPROPERTY(BlueprintAssignable)
	//FOnNeedToAddBeatmapsObjects OnNeedToAddBeatmapObjects;

	/// <summary>
	/// MapData
	/// Target actor
	/// beat
	/// renderDistance
	/// </summary>
	SDelegate<FMapData*, ABeatCell*, float, float> OnNeedToAddBeatmapObjects;

	UPROPERTY(BlueprintAssignable)
	FOnNeedToDestroyBeatmapObjects OnNeedToDestroyBeatmapObjects;

	UPROPERTY(BlueprintAssignable)
	FOnNeedToUpdateMapperPawnPosition OnNeedToUpdateMapperPawnPosition;

	UPROPERTY(BlueprintAssignable)
	FOnNeedToResetMapperPawnTransform OnNeedToResetMapperPawnTransform;

	UPROPERTY()
	TObjectPtr<UStaticMesh> BombMesh;
	UPROPERTY()
	TObjectPtr <UStaticMesh> DotMesh;
	UPROPERTY()
	TObjectPtr <UStaticMesh> CubeMesh;
	UPROPERTY()
	TObjectPtr <UMaterialInterface> WallMaterial;
	UPROPERTY()
	TObjectPtr <UMaterialInstance> GlobalNoteMaterial;
	UPROPERTY()
	TObjectPtr <UMaterialInstance> RightNoteMaterial;
	UPROPERTY()
	TObjectPtr <UMaterialInstance> LeftNoteMaterial;
	UPROPERTY()
	TObjectPtr <UMaterialInstance> PassedRightNoteMaterial;
	UPROPERTY()
	TObjectPtr <UMaterialInstance> PassedLeftNoteMaterial;
	UPROPERTY()
	TObjectPtr <UMaterialInstance> BombMaterial;
	UPROPERTY()
	TObjectPtr <UMaterialInterface> SoundvisMaterial;
	UPROPERTY()
	TObjectPtr<UMaterialInterface> OutlineMaterial;
	UPROPERTY()
	TObjectPtr<UMaterialInterface> GridOutlineMaterial;
	UPROPERTY()
	TObjectPtr <USoundWave> HitSound;

	UPROPERTY()
	TObjectPtr<UMMAConfig> Config;

	FHistory History;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float LowPassFilter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float HighPassFilter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SelectionFirstBPM;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SelectionEndBPM;

	UFUNCTION()
	void Play();

	UFUNCTION()
	void Stop();

	//UFUNCTION()
	void SetMap(FMapInfo* info, FString diff, FString mode);

	UFUNCTION()
	void AddTime(float time);

	UFUNCTION()
	void SetTime(float time);

	UFUNCTION()
	void GenerateBeatGrid();

	UFUNCTION()
	void ResetCurrent();

	UFUNCTION()
	void GenerateVisMesh();

	UFUNCTION()
	void UpdateVisMesh();

	UFUNCTION()
	void UpdateBeatGrid();

	UFUNCTION()
	UMaterialInstance* UpdateNoteMaterial(UMaterialInterface* noteMaterial, int type, bool passed, bool isSelected);

	UFUNCTION()
	void SetCurrentScene(FString sceneName);

	UFUNCTION()
	void DisplayEditModeSettings();

	UFUNCTION()
	void PlayHitSound();

	UFUNCTION(BlueprintCallable)
	void SetFilter(float low, float high);

	UFUNCTION()
	void SortMapContent();

//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION()
	float GetBeat();

	UFUNCTION()
	float GetDeltaBeat();

	UFUNCTION()
	float GetPlayTime();

	UFUNCTION()
	bool IsPlaying();

	FMapData* GetMapData();

	//UFUNCTION()
	FMapInfo* GetMapInfo();

	FMapDifficulty* GetMapDifficulty();

	UFUNCTION()
	FString GetCurrentSceneName();

	UFUNCTION()
	ABeatCell* GetBeatCell();

	UFUNCTION()
	float BeatToTime(float beat);

	int GetNoteIndexByBeat(float beat, float maxError, int baseIndex);

	TArray<int> GetAllNotesIndexInSectionExclusive(float beat, float length, int baseIndex);

	TArray<int> GetAllNotesIndexInSectionInclusive(float beat, float length, int baseIndex);

	TArray<FNoteData*> GetSelectedNotes();

	FNoteData* GetLastSameColorNote(FNoteData* current);

	FNoteData* GetLastSameColorNote(float beat, int type);

//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Save();
};

ACController* ACController::Instance = nullptr;

static ACController* GetMappingController() {
	return ACController::Instance;
}

static UMMAConfig* GetConfig() {
	return GetMappingController()->Config;
}