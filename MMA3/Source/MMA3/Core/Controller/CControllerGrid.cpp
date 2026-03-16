#include "CController.h"
#include "MMA3/Core/CObjectsSpawner.h"

void ACController::UpdateBeatGrid() {
	if (BeatCells && MapInfo->Song) {
		double l_YPosition = (PlayingTime * (MapInfo->_beatsPerMinute / 60.f)) * 100 * (double)GetConfig()->MappingGridScale;

		//UE_LOG(LogTemp, Warning, TEXT("Position: %lf, Playtime: %f, BPM: %f, GridScale: %f"), l_YPosition, PlayingTime, MapInfo.BPM, GetConfig()->MappingGridScale);
		if (l_YPosition == 0 && PlayingTime > 0) {
			Stop();
			UE_LOG(LogTemp, Error, TEXT("Position is 0, why ???"));
			return;
		}

		BeatCells->SetActorLocation(FVector(0, -l_YPosition, 0));
		/*OnNeedToAddBeatmapObjects.Broadcast(&MapContent, BeatCells, GetBeat(), RenderDistance);*/
		Cast<ACObjectsSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), ACObjectsSpawner::StaticClass()))->SpawnObjects(MapContent, BeatCells, GetBeat(), RenderDistance);
	}
}

void ACController::GenerateBeatGrid()
{
	if (MapInfo->Song) {
		float l_OriginalBeatCount = BeatmapUtils::CalculateBeatCount(MapInfo->Song->Duration, MapInfo->_beatsPerMinute);

		if (BeatCells == nullptr) {
			BeatCells = GetWorld()->SpawnActor<ABeatCell>();
		}
		else {
			OnNeedToDestroyBeatmapObjects.Broadcast();
		}

		BeatCells->SetLength(l_OriginalBeatCount);

		UpdateBeatGrid();
	}
}

void ACController::ResetCurrent() {
	GetObjectsSpawner()->DestroyObjects();

	SetTime(FMath::Clamp(PlayingTime + 0.1f, 0, SongDuration));
	UpdateBeatGrid();
	SetTime(FMath::Clamp(PlayingTime - 0.1f, 0, SongDuration));
	UpdateBeatGrid();

	//GetObjectsSpawner()->SpawnObjects(MapContent, BeatCells, GetBeat(), RenderDistance);

	//UpdateBeatGrid();
}