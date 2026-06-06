// Fill out your copyright notice in the Description page of Project Settings.


#include "CNoteTool.h"
#include <Kismet/GameplayStatics.h>
#include "MMA3/MMAConfig.h"

void ACNoteTool::OnUpdate(const FVector& position)
{
	Super::OnUpdate(position);
	
	SetActorLocation(position);
}

ACNoteTool::ACNoteTool() {
	ConstructorHelpers::FObjectFinder<UStaticMesh> l_ArrowMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/NoteBody_Cylinder.NoteBody_Cylinder'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> l_DotMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/NoteBody_Cylinder_001.NoteBody_Cylinder_001'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance>l_NoteMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/Mapping/M_NoteInstance.M_NoteInstance'"));

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow Mesh"));
	ArrowMesh->SetStaticMesh(l_ArrowMesh.Object);
	ArrowMesh->SetupAttachment(ToolMeshComp);
	ArrowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ToolMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DotMesh = l_DotMesh.Object;

	ArrowStaticMesh = l_ArrowMesh.Object;

	NoteMaterial = l_NoteMaterial.Object;

	CustomScale = FVector(0.17f, 0.17f, 0.17f);

	Type = 1;
	Direction = 0; 
}

void ACNoteTool::Tick(float delta) {
	Super::Tick(delta);

	ArrowMesh->SetStaticMesh(Direction == 8 ? DotMesh : ArrowStaticMesh);
}

void ACNoteTool::OnFinishUsing(FVector position) {
	if (!GetMappingController()->GetMapDifficulty()->HasRequirement(EDefaultRequirement::MappingExtensions) &&
		!GetMappingController()->GetMapDifficulty()->HasRequirement(EDefaultRequirement::NoodleExtensions)) {

		int l_LineIndex = -((position.X + 13) / 25) + 4;
		int l_LineLayer = (position.Z) / 25;
		
		FNoteData* l_Note = new FNoteData();
		l_Note->Line = l_LineIndex;
		l_Note->Layer = l_LineLayer;
		l_Note->Direction = Direction;
		l_Note->Beat = GetMappingController()->GetBeat();
		l_Note->Type = Type;

		//GetObjectsSpawner()->SpawnObject<ACNote, FNoteData>(l_Note, Cast<ABeatCell>(UGameplayStatics::GetActorOfClass(GetWorld(), ABeatCell::StaticClass())));
		GetMappingController()->MapContent->_notes.Add(l_Note);
		GetMappingController()->SortMapContent();

		GetMappingController()->ResetCurrent();

		GetMappingController()->History.Remember(new FNoteAction(l_Note));
	}
}

UStaticMesh* ACNoteTool::GetToolMesh() {
	

	if (Type == 3) return GetMappingController()->BombMesh;
	else return GetMappingController()->CubeMesh;
}

UMaterialInterface* ACNoteTool::GetToolMaterial() {

	UMaterialInstance* l_Material = nullptr;
	if (Type != 3) {
		 l_Material = GetMappingController()->UpdateNoteMaterial(NoteMaterial, Type, true, false);
	}
	else {
		l_Material = GetMappingController()->BombMaterial;
	}

	return l_Material;
}

FVector ACNoteTool::GetCustomScale()
{
	if (Type != 3) {
		return FVector(0.17f, 0.17f, 0.17f);
	}
	else {
		return FVector(0.12f, 0.12f, 0.12f);
	}
}
