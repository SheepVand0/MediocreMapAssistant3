// Fill out your copyright notice in the Description page of Project Settings.


#include "CSelectionTool.h"

#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "MMA3/Core/CObjectsSpawner.h"
#include "MMA3/Core/Controller/CController.h"
#include "MMA3/Core/MapperPawn/CMapperPawn.h"

ACSelectionTool::ACSelectionTool()
{
	ConstructorHelpers::FObjectFinder<UMaterialInstance>selectionBoxMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/MSelectionInst.MSelectionInst'"));
	ProceduralMeshMaterial = selectionBoxMaterial.Object;
	
	SelectionBox = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Selection box"));
	SetRootComponent(SelectionBox);
	
	bInstantUse = false;
}

void ACSelectionTool::OnFinishUsing(FVector position)
{
	Super::OnFinishUsing(position);
	
	OnFinishedSelection.Broadcast();
}

void ACSelectionTool::OnStartUsing(FVector position)
{
	Super::OnStartUsing(position);
	
	SetActorLocation(position);
	
	AttachToActor(GetMappingController()->GetBeatCell(), FAttachmentTransformRules::KeepWorldTransform);
	
	StartTime = GetMappingController()->GetPlayTime();
	EndTime = GetMappingController()->GetPlayTime();
	
	StartX = -((GetMapperPawn()->CursorPosX) / 25) + 3;
	StartZ = GetMapperPawn()->CursorPosZ / 25;
}

void ACSelectionTool::OnUpdateUse(const FVector& position)
{
	Super::OnUpdateUse(position);
	
	
	EndTime = GetMappingController()->GetPlayTime();
	FVector box = position - GetActorLocation();
	
	TArray<FVector> vertices;
	TArray<int32> triangles;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FProcMeshTangent> tangents;
	
	ACController* controller = GetMappingController();
	
	if (!controller->MapContent) return;
	
	int32 startIndex = 0;
	
	TArray<int32> keys;
	GetObjectsSpawner()->DisplayedNotes.GetKeys(keys);
	if (keys.Num())
	{
		startIndex = keys[0];
	}
	
	 FBox boxExtents = FBox::BuildAABB(GetActorLocation(), GetActorLocation() + box);
	
	//int cursorX = FMath::Max(position.X / 25, GetActorLocation().X / 25);
	//int cursorY = FMath::Max(position.Y / 25, GetActorLocation().Y / 25);
	
	int cursorX = -((GetMapperPawn()->CursorPosX) / 25) + 3;
	int cursorZ = (GetMapperPawn()->CursorPosZ) / 50;
	
	//int startX = FMath::Min(position.X / 25, GetActorLocation().X / 25);
	//int startY = FMath::Min(position.Y / 25, GetActorLocation().Y / 25);
	
	for (int32 x = 0;x < GetObjectsSpawner()->DisplayedNotes.Num();x++)
	{
		ACNote* note = GetObjectsSpawner()->DisplayedNotes[keys[x]];
		
		UE_LOG(LogTemp, Display, TEXT("1: %d, 2: %d, 3: %d, 4: %d"), cursorX, StartX, cursorZ, StartZ);
		if (note->NoteData->Line >= StartX && note->NoteData->Line <= cursorX && note->NoteData->Layer >= StartZ && note->NoteData->Layer <= cursorZ)
		{
			UE_LOG(LogTemp, Display, TEXT("It's inside !"));
			GetMappingController()->AddNoteToSelection(note);
		} else
		{
			GetMappingController()->RemoveToSelection(note);
		}
	}
	
	UKismetProceduralMeshLibrary::GenerateBoxMesh(box, vertices, triangles, normals, uvs, tangents);
	
	uvs[0] = uvs[4] = uvs[8] = uvs[12] = uvs[16] = uvs[20] = FVector2D(0.f, 0.f);
	uvs[1] = uvs[5] = uvs[9] = uvs[13] = uvs[17] = uvs[21] = FVector2D(0.f, box.Y / 1000.0);
	uvs[2] = uvs[6] = uvs[10] = uvs[14] = uvs[18] = uvs[22] = FVector2D(box.X / 1000.0, box.Y / 1000.0);
	uvs[3] = uvs[7] = uvs[11] = uvs[15] = uvs[19] = uvs[23] = FVector2D(box.X / 1000.0, 0.f);
	
	SelectionBox->SetRelativeLocation_Direct(FVector(box.X / 2.f, 0, 0));
	
	SelectionBox->CreateMeshSection(0, vertices, triangles, normals, uvs, TArray<FColor>(), tangents, false);
	SelectionBox->SetMaterial(0, ProceduralMeshMaterial);
}


