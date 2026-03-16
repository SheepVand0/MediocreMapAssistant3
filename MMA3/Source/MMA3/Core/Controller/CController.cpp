// Fill out your copyright notice in the Description page of Project Settings.


#include "CController.h"
#include <MMA3/Core/MapperPawn/CMapperPawn.h>

void ACController::SetCurrentScene(FString sceneName)
{
	CurrentScene = sceneName;
}

void ACController::DisplayEditModeSettings()
{
	EditModeWidget->SwitchSettingsWidget();
}

void ACController::PlayHitSound()
{
	HitSoundAudioComponent->Stop();
	HitSoundAudioComponent->VolumeMultiplier = GetConfig()->HitSoundVolume;
	HitSoundAudioComponent->Play(0.0f);
}

void ACController::SetFilter(float low, float high)
{
	LowPassFilter = low;
	HighPassFilter = high;
}

void ACController::SortMapContent()
{
	GetMappingController()->MapContent->_notes.Sort([](const FNoteData& A, const FNoteData& B) {
		return A < B;
		});

	GetMappingController()->MapContent->_walls.Sort([](const FWallData& A, const FWallData& B) {
		return A < B;
		});
}

void ACController::SetMap(FMapInfo* mapInfo, FString difficulty, FString mode) {
	if (mapInfo->Song == nullptr) { 
		UE_LOG(LogScript, Error, TEXT("Song is not valid")); 
		return; 
	}

	MapInfo = mapInfo;

	FString l_MapData;
	FMapDifficulty* l_Difficulty = nullptr;
	for (int l_i = 0; l_i < MapInfo->DifficultyBeatmapSets.Num(); l_i++) {
		if (MapInfo->DifficultyBeatmapSets[l_i]->Name == mode) {
			for (int l_i1 = 0; l_i1 < MapInfo->DifficultyBeatmapSets[l_i]->DifficultyBeatmaps.Num(); l_i1++) {
				auto l_Diff = MapInfo->DifficultyBeatmapSets[l_i]->DifficultyBeatmaps[l_i1];
				if (l_Diff->_difficulty == difficulty) {
					l_Difficulty = l_Diff;
				}
			}
			break;
		}
	}


	if (!l_Difficulty) return;

	Difficulty = l_Difficulty;

	FFileHelper::LoadFileToString(l_MapData, *FString(mapInfo->MapPath + "\\" + l_Difficulty->_beatmapFilename));
	MapContent = new FMapData();
	MapContent->FromJson(l_MapData);

	SongDuration = mapInfo->Song->Duration;
	PlayingTime = 0;

	GenerateBeatGrid();

	////////////////////////////////////////////////////////////////////////////////////////////
	// Note spawning

	//OnNeedToAddBeatmapObjects.Broadcast(&MapContent, BeatCells, 0, RenderDistance);

	Cast<ACObjectsSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), ACObjectsSpawner::StaticClass()))->SpawnObjects(MapContent, BeatCells, 0, RenderDistance);
	OnNeedToResetMapperPawnTransform.Broadcast();

	EditModeWidget->OnTimeChanged.RemoveAll(this);
	EditModeWidget->OnTimeChanged.AddDynamic(this, &ACController::WidgetTimeSliderChanged);
	EditModeWidget->Init(SongDuration);
	FInputModeGameAndUI l_InputMode;
	l_InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerControllerReference->SetInputMode(l_InputMode);

	GenerateVisMesh();

	GetMapperPawn()->InputComponent->SetActive(true);
}

void ACController::GenerateVisMesh()
{
	URenderSoundVisualization::GenerateVisMesh(
		SoundVisMesh,
		GetConfig()->SoundVisVerticesX,
		GetConfig()->SoundVisVerticesY, 
		((float)SOUND_VIS_VERTICES_X / GetConfig()->SoundVisVerticesX) * 3,
		(float)SOUND_VIS_VERTICES_Y / GetConfig()->SoundVisVerticesY, 
		SoundVisVertices, 
		SoundVisVertexColors
	);
}

void ACController::UpdateVisMesh() {
	//UE_LOG(LogTemp, Display, TEXT("[UpdateVisMesh] Started"));
	URenderSoundVisualization::RenderSoundVis(
		SoundVisMesh,
		MapInfo->Song,
		SoundVisVertices,
		SoundVisVertexColors,
		PlayingTime,
		(60.f / MapInfo->_beatsPerMinute) * ((float)SOUND_VIS_VERTICES_X / 100.f) * 3 / GetConfig()->MappingGridScale, 1.f / GetConfig()->SoundVisDetail,
		GetConfig()->SoundVisVerticesX,
		GetConfig()->SoundVisVerticesY);
	//UE_LOG(LogTemp, Display, TEXT("[UpdateVisMesh] Finished correctly"));
}