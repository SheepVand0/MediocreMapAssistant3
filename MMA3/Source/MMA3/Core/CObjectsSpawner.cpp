// Fill out your copyright notice in the Description page of Project Settings.


#include "CObjectsSpawner.h"
#include "MMA3/Core/Controller/CController.h"

// Sets default values
ACObjectsSpawner::ACObjectsSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Instance = this;
}

// Called when the game starts or when spawned
void ACObjectsSpawner::BeginPlay()
{
	Super::BeginPlay();

	//GetMappingController()->OnNeedToAddBeatmapObjects.Bind(SMethodPtr(this, &ACObjectsSpawner::SpawnObjects));
	GetMappingController()->OnNeedToDestroyBeatmapObjects.AddDynamic(this, &ACObjectsSpawner::DestroyObjects);
}

// Called every frame
void ACObjectsSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACObjectsSpawner::SpawnObjects(FMapData* mapContent, ABeatCell* beatCell, float beat, float renderDistance) {
	float minBeat = beat - (renderDistance * 0.2f);
	float maxBeat = beat + (renderDistance * 0.8f);

	if (renderDistance == 0.f) {
		DestroyObjects();
		
		return;
	}

	/*if (beat == 0) {
		DestroyObjects();
		LastNoteBeatIndex = 0;
		LastWallBeatIndex = 0;
		LastArcBeatIndex = 0;
		LastControllerBeat = 0.f;
	}*/

	if (beat < LastControllerBeat) {
		int l_Index = LastNoteBeatIndex;

		if (mapContent->_notes.Num() > 1) {
			FNoteData* l_Item;
			while ((l_Item = mapContent->_notes[l_Index])->Beat >= minBeat && l_Index > 0) {
				l_Index--;
			}
			LastNoteBeatIndex = l_Index; 
		}
		else {
			LastNoteBeatIndex = 0;
		}

		if (mapContent->_walls.Num() > 1) {
			l_Index = LastWallBeatIndex;
			FWallData* l_Wall;
			while ((l_Wall = mapContent->_walls[l_Index])->Beat + l_Wall->Duration >= minBeat && l_Index > 0) {
				l_Index--;
			}
			LastWallBeatIndex = l_Index;
		}
		else {
			LastWallBeatIndex = 1;
		}

		if (mapContent->_arcs.Num() > 1) {
			l_Index = LastArcBeatIndex;
			FArcData* l_Arc;
			while ((l_Arc = mapContent->_arcs[l_Index])->Beat + (l_Arc->TailBeat - l_Arc->Beat) >= minBeat && l_Index > 0) {
				l_Index--;
			}
			LastArcBeatIndex = l_Index;
		}
		else {
			LastArcBeatIndex = 1;
		}
	}

	TArray<int32> l_NotesKeys;
	DisplayedNotes.GetKeys(l_NotesKeys);

	for (int l_i = 0; l_i < DisplayedNotes.Num(); l_i++) {

		int32 l_Key = l_NotesKeys[l_i];

		if (DisplayedNotes[l_Key] != nullptr) {
			if (DisplayedNotes[l_Key]->NoteData->Beat < minBeat || DisplayedNotes[l_Key]->NoteData->Beat > maxBeat) {
				DisplayedNotes[l_Key]->Destroy(true);
				DisplayedNotes.Remove(l_Key);
			}
		}
		else {
			DisplayedNotes.Remove(l_Key);
		}
	}

	TArray<int32> l_WallsKeys;
	DisplayedWalls.GetKeys(l_WallsKeys);
	
	for (int l_i = 0; l_i < DisplayedWalls.Num(); l_i++) {
		int32 l_Key = l_WallsKeys[l_i];

		FWallData* l_DisplayedWallData = DisplayedWalls[l_Key]->WallData;
		float l_WallBeatEnd = l_DisplayedWallData->Beat + l_DisplayedWallData->Duration;

		if (DisplayedWalls[l_Key] != nullptr) {

			if ((l_WallBeatEnd >= maxBeat && l_DisplayedWallData->Beat <= minBeat)
				|| (l_WallBeatEnd <= maxBeat && l_WallBeatEnd >= minBeat)
				|| (l_DisplayedWallData->Beat >= minBeat && l_DisplayedWallData->Beat <= maxBeat)) {

				DisplayedWalls[l_Key]->SetExtremumsBeats(minBeat, maxBeat);
				DisplayedWalls[l_Key]->BuildMesh();
			}
			else {
				DisplayedWalls[l_Key]->Destroy(true);
				DisplayedWalls.Remove(l_Key);
			}
		}
		else {
			DisplayedWalls.Remove(l_Key);
		}
	}

	TArray<int32> l_ArcsKeys;
	DisplayedArcs.GetKeys(l_ArcsKeys);

	for (int l_i = 0; l_i < DisplayedArcs.Num(); l_i++) {
		int32 l_Key = l_ArcsKeys[l_i];

		FArcData l_DisplayedArc = DisplayedArcs[l_i]->ArcData;
		
		if (DisplayedArcs[l_Key] != nullptr) {

			if ((l_DisplayedArc.TailBeat < minBeat) || (l_DisplayedArc.Beat > maxBeat)) {
				DisplayedArcs[l_i]->Destroy(true);
				DisplayedArcs.Remove(l_Key);
			}

		}
	}

////////////////////////////////////////////////////////////////////////////////

	for (int l_i = LastNoteBeatIndex; l_i < mapContent->_notes.Num(); l_i++) {
		FNoteData* l_NoteData = mapContent->_notes[l_i];

		if (l_NoteData->Beat > maxBeat) {
			LastNoteBeatIndex = FMath::Max(0, l_i - 1);
			break;
		}

		if (((l_NoteData->Beat < beat && l_NoteData->Beat >= minBeat) ||
			(l_NoteData->Beat > beat && l_NoteData->Beat < maxBeat)) && !l_NotesKeys.Contains(l_i)) {

			DisplayedNotes.Add(l_i, SpawnObject<ACNote, FNoteData>(mapContent->_notes[l_i], beatCell));
		}
	}

	for (int l_i = LastWallBeatIndex; l_i < mapContent->_walls.Num(); l_i++) {
		FWallData* l_WallData = mapContent->_walls[l_i];
		float l_WallBeatEnd = l_WallData->Beat + l_WallData->Duration;

		if (l_WallData->Beat >= maxBeat) {
			LastWallBeatIndex = FMath::Max(0, l_i - 1);
			break;
		}

		if (((l_WallBeatEnd > maxBeat && l_WallData->Beat < minBeat)
			|| (l_WallBeatEnd < maxBeat && l_WallBeatEnd > minBeat)
			|| (l_WallData->Beat > minBeat && l_WallData->Beat < maxBeat))
			&& (!l_WallsKeys.Contains(l_i))) {

			ACWall* l_Wall = SpawnObject<ACWall, FWallData>(l_WallData, beatCell);
			l_Wall->SetExtremumsBeats(minBeat, maxBeat);
			l_Wall->BuildMesh();
			DisplayedWalls.Add(l_i, l_Wall);
		}
	}

	for (int l_i = LastArcBeatIndex; l_i < mapContent->_arcs.Num(); l_i++) {
		FArcData* l_Arc = mapContent->_arcs[l_i];
		
		if (l_Arc->Beat > maxBeat) {
			LastWallBeatIndex = FMath::Max(0, l_i - 1);
			break;
		}

		if ((((l_Arc->TailBeat >= minBeat) && (l_Arc->TailBeat <= maxBeat)) || ((l_Arc->Beat < maxBeat && l_Arc->Beat > minBeat))) && (!l_ArcsKeys.Contains(l_i))) {
			//ACArc* l_ArcObj = SpawnObject<ACArc, FArcData>(l_Arc, beatCell);
			//DisplayedArcs.Add(l_i, l_ArcObj);
		}
	}

	LastControllerBeat = beat;
}

void ACObjectsSpawner::DestroyObjects()
{
	
	TArray<TObjectPtr<ACNote>> l_Notes;
	DisplayedNotes.GenerateValueArray(l_Notes);
	for (int l_i = 0; l_i < DisplayedNotes.Num(); l_i++) {
		if (!l_Notes[l_i]) continue;
		if (l_Notes[l_i]->GetIsPreview()) continue;
		l_Notes[l_i]->Destroy();
	}

	TArray<TObjectPtr<ACWall>> l_Walls;
	DisplayedWalls.GenerateValueArray(l_Walls);
	for (int l_i = 0; l_i < DisplayedWalls.Num(); l_i++) {
		l_Walls[l_i]->Destroy();
	}

	TArray<TObjectPtr<ACArc>> l_Arcs;
	DisplayedArcs.GenerateValueArray(l_Arcs);
	for (int l_i = 0; l_i < DisplayedArcs.Num(); l_i++) {
		l_Arcs[l_i]->Destroy();
	}

	DisplayedNotes.Empty();
	DisplayedWalls.Empty();
	DisplayedArcs.Empty();

	LastNoteBeatIndex = 0;
	LastWallBeatIndex = 0;
	LastArcBeatIndex = 0;
	LastControllerBeat = 0.f;
	LastControllerBeat = 0.f;
}

void ACObjectsSpawner::RemoveNote(int32 index) {

	if (DisplayedNotes.Contains(index)) {
		DisplayedNotes.Find(index)->Get()->Destroy();
		DisplayedNotes.Remove(index);
		TMap<int32, TObjectPtr<ACNote>> l_Array = DisplayedNotes;
		DisplayedNotes.Empty();
		TArray<int32> l_Keys;
		l_Array.GetKeys(l_Keys);
		for (auto x : l_Keys) {
			if (x > index) {
				DisplayedNotes.Add(x - 1, l_Array.Find(x)->Get());
			}
			else {
				DisplayedNotes.Add(x, l_Array.Find(x)->Get());
			}
		}
	}

}

template<class t_ObjectType, typename t_StructType>
inline t_ObjectType* ACObjectsSpawner::SpawnObject(t_StructType* objectData, ABeatCell* beatCell)
{
	FTransform l_Transform = FTransform(t_ObjectType::CalculateObjectLocation(*objectData));
	auto l_Object = Cast<t_ObjectType>(GetWorld()->SpawnActor(t_ObjectType::StaticClass()));
	l_Object->AttachToActor((AActor*)beatCell, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	l_Object->SetData(objectData);
	return l_Object;
}