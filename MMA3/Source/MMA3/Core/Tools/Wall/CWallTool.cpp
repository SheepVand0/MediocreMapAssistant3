// Fill out your copyright notice in the Description page of Project Settings.


#include "CWallTool.h"

ACWallTool::ACWallTool()
{
	bInstantUse = false;	
}

void ACWallTool::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	/// TODO: Rewrite (doesn't work well)
	if (IsPlacing) {
		if (GetMappingController()->GetBeat() - LastBeat != 0) {
			float l_Duration = GetMappingController()->GetBeat() - TempWall->WallData->Beat;

			TempWall->WallData->Duration = l_Duration;
		}

		int l_LineIndex = 3 - ((GetActorLocation().X + 25 + 13) / 25) + 1;
		int l_LineLayer = (GetActorLocation().Z) / 25;

		TempWall->WallData->Width = (l_LineIndex - TempWall->WallData->Line);

		if (TempWall->WallData->Line > (l_LineIndex - 1)) {
			TempWall->WallData->Line += TempWall->WallData->Width;
			TempWall->WallData->Width = -(TempWall->WallData->Width) + 1;
		}

		TempWall->WallData->Layer = FMath::Min(l_LineLayer, 2);
		TempWall->WallData->Height = l_LineLayer == 0 ? 5 : 4;
		TempWall->BuildMesh();

		UpdateWallLocation();
	}

	LastBeat = GetMappingController()->GetBeat();
}

void ACWallTool::UpdateWallLocation()
{
	if (TempWall == nullptr) return;

	FVector l_Pos = ACWall::CalculateWallLocation(*TempWall->WallData);
	l_Pos *= GetConfig()->MappingGridScale;
	l_Pos.Y -= GetMappingController()->GetBeat() * GetConfig()->MappingGridScale * 100;

	TempWall->SetActorLocation(l_Pos);
}

void ACWallTool::OnUpdate(const FVector& position)
{
	Super::OnUpdate(position);
	
	SetActorLocation(position);
}

UStaticMesh* ACWallTool::GetToolMesh()
{
	return nullptr;
}

UMaterialInterface* ACWallTool::GetToolMaterial()
{
	return nullptr;
}

void ACWallTool::OnFinishUsing(FVector location)
{
	PrimaryActorTick.bCanEverTick = true;

	if (!IsPlacing) {
		int l_LineIndex = 3 - ((GetActorLocation().X + 25 + 13) / 25);
		int l_LineLayer = FMath::Min((GetActorLocation().Z) / 25, 2);

		FWallData* l_TempWallData ;

		if (TempWall) {
			l_TempWallData = TempWall->WallData;
		}
		else {
			l_TempWallData = new FWallData();
		}

		l_TempWallData->Beat = GetMappingController()->GetBeat();
		l_TempWallData->Width = 1;
		l_TempWallData->Height = 5;
		l_TempWallData->Duration = 0;
		l_TempWallData->Layer = l_LineLayer;
		l_TempWallData->Line = l_LineIndex;

		if (TempWall == nullptr) {
			
			FTransform l_Transform = FTransform(FRotator(0, 0, 0), ACWall::CalculateWallLocation(*l_TempWallData), FVector(1, 1, 1));
			TempWall = Cast<ACWall>(GetWorld()->SpawnActor(ACWall::StaticClass(), &l_Transform));
		}
		else {
			TempWall->WallMesh->SetVisibility(true, true);
		}

		TempWall->WallData = l_TempWallData;
		TempWall->BuildMesh();
		TempWall->SetExtremumsBeats(0, INT_MAX);

		IsPlacing = true;
	}
	else {
		GetMappingController()->MapContent->_walls.Add(TempWall->WallData);
		GetMappingController()->SortMapContent();
		GetMappingController()->ResetCurrent();
		TempWall->Destroy();
		

		GetMappingController()->History.Remember(new FWallAction(TempWall->WallData));

		IsPlacing = false;
	}
}
