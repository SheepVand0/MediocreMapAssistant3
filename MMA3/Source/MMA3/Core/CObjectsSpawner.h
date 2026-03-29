// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "MMA3/Core/BeatmapObjects/CNote.h"
#include "MMA3/Core/BeatmapObjects/CWall.h"
#include "MMA3/Core/BeatmapObjects/CArc.h"
#include "CObjectsSpawner.generated.h"

UCLASS()
class MMA3_API ACObjectsSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACObjectsSpawner();

	static ACObjectsSpawner* Instance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int LastNoteBeatIndex;
	int LastWallBeatIndex;
	int LastArcBeatIndex;

	float LastControllerBeat;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TMap<int32, TObjectPtr<ACNote>> DisplayedNotes;
	TMap<int32, TObjectPtr<ACWall>> DisplayedWalls;
	TMap<int32, TObjectPtr<ACArc>> DisplayedArcs;

	void SpawnObjects(FMapData* map, ABeatCell* beatCell, float beat, float renderDistance);

	UFUNCTION()
	void DestroyObjects();

	void RemoveNote(int32 index);

	template<class t_ObjectType, typename t_StructType>
	t_ObjectType* SpawnObject(t_StructType* objectData, ABeatCell* beatCell);

	int GetLastDisplayedNoteIndex() {
		return LastNoteBeatIndex;
	}

};

ACObjectsSpawner* ACObjectsSpawner::Instance = nullptr;

static ACObjectsSpawner* GetObjectsSpawner() {
	return ACObjectsSpawner::Instance;
}


