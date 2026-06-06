// Fill out your copyright notice in the Description page of Project Settings.


#include "CNote.h"

#include "MMA3/Core/Controller/CController.h"

TMap<int, int> ACNote::RotationByCutDirection;

// Sets default values
ACNote::ACNote() : ActorBeat(0), LastActorBeat(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_NoteMaterial(TEXT(
		"/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/Mapping/M_NoteInstance.M_NoteInstance'"));
	ConstructorHelpers::FObjectFinder<UMaterial> l_DefaultNoteMaterial(TEXT("/Script/Engine.Material'/Game/Assets/Materials/Mapping/M_Note.M_Note'"));
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> l_Cube(
		TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/NoteBody_Cube.NoteBody_Cube'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> l_Arrow(
		TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/NoteBody_Cylinder.NoteBody_Cylinder'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_BombMat(
		TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/Mapping/M_Bomb.M_Bomb'"));

	NoteMaterial = l_NoteMaterial.Object;
	BombMaterial = l_BombMat.Object;
	OutlineMaterial = l_DefaultNoteMaterial.Object;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	SetRootComponent(CubeMesh);
	CubeMesh->SetStaticMesh(l_Cube.Object);
	CubeMesh->SetMaterial(0, l_NoteMaterial.Object);

	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(CubeMesh);
	Arrow->SetStaticMesh(l_Arrow.Object);
	Arrow->SetRelativeScale3D(FVector(1, 1, 1));

	SetActorScale3D(FVector(0.17f, 0.17f, 0.17f));

	InitAngles();
}

ACNote::~ACNote()
{
}


// Called when the game starts or when spawned
void ACNote::BeginPlay()
{
	Super::BeginPlay();

	LastActorBeat = GetMappingController()->GetBeat();
}


// Called every frame
void ACNote::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ActorBeat = GetMappingController()->GetBeat();

	TArray<FNoteData*> selection = GetMappingController()->GetSelectedNotes(); 
	
	if (!bIsSelected && selection.Num())
	{
		bIsSelected = selection.Contains(NoteData);
	}
	
	bool isSelected = bIsSelected;

	if (ActorBeat > LastActorBeat)
	{
		if ((ActorBeat >= NoteData->Beat) && (LastActorBeat <= NoteData->Beat) && GetMappingController()->IsPlaying() &&
			!Passed)
		{
			GetMappingController()->PlayHitSound();
			GetMappingController()->BroadcastNotePassed(NoteData);
		}
	}

	if (!IsPreview)
	{
		if ((NoteData->Beat < GetMappingController()->GetBeat() && NoteData->Type != 3) && (!Passed))
		{
			Passed = true;

			UpdateNoteMaterial(isSelected);
		}

		if ((NoteData->Beat > GetMappingController()->GetBeat() && NoteData->Type != 3) && Passed)
		{
			Passed = false;

			UpdateNoteMaterial(isSelected);
		}
	}
	else
	{
		Passed = true;

		UpdateNoteMaterial();
	}

	LastActorBeat = ActorBeat;
}

FVector ACNote::CalculateObjectLocation(FNoteData noteData)
{
	return CalculateNoteLocation(noteData);
}

FVector ACNote::CalculateNoteLocation(FNoteData noteData)
{
	return FVector(75.f - (noteData.Line * 25) + (25.f / 2), noteData.Beat * 100, noteData.Layer * 25 + (25.f / 2));
}

void ACNote::UpdateNoteMaterial(bool isSelected)
{
	if (IsActorBeingDestroyed()) return;

	UMaterialInstance* l_ColoredNotMaterial = GetMappingController()->UpdateNoteMaterial(
		NoteData->Type != 3 ? (isSelected ? OutlineMaterial : NoteMaterial) : BombMaterial, NoteData->Type, Passed, isSelected);
	CubeMesh->SetMaterial(0, l_ColoredNotMaterial);
}

void ACNote::InitAngles()
{
	if (RotationByCutDirection.Num() == 0)
	{
		RotationByCutDirection.Add(0);
		RotationByCutDirection.Add(1);
		RotationByCutDirection.Add(2);
		RotationByCutDirection.Add(3);
		RotationByCutDirection.Add(4);
		RotationByCutDirection.Add(5);
		RotationByCutDirection.Add(6);
		RotationByCutDirection.Add(7);
		RotationByCutDirection[0] = 180 % 360;
		RotationByCutDirection[1] = 0;
		RotationByCutDirection[2] = 270 % 360;
		RotationByCutDirection[3] = 90 % 360;
		RotationByCutDirection[4] = 225 % 360;
		RotationByCutDirection[5] = 135 % 360;
		RotationByCutDirection[6] = 315 % 360;
		RotationByCutDirection[7] = 45 % 360;
	}
}

int ACNote::CutDirectionFromAngle(int angle)
{
	InitAngles();

	const int32* l_Result = RotationByCutDirection.FindKey(angle % 360);
	if (l_Result == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("Angle is not in Cut Directions, angle: %d"), angle);
		return 0;
	}
	return *l_Result;
}

void ACNote::SetData(FNoteData* noteData)
{
	NoteData = noteData;
	SetActorRelativeLocation(CalculateNoteLocation(*noteData));
	if (NoteData->Type == 3)
	{
		Arrow->SetVisibility(false);
		CubeMesh->SetStaticMesh(GetMappingController()->BombMesh);
		CubeMesh->SetRelativeScale3D(FVector(-0.08f, -0.08f, -0.08f));
		CubeMesh->SetMaterial(0, GetMappingController()->BombMaterial);
	}
	else if (noteData->Direction == 8)
	{
		Arrow->SetStaticMesh(GetMappingController()->DotMesh);
		SetActorRotation(FRotator::MakeFromEuler(FVector(0, 0, -90)));
	}
	else
	{
		SetActorRotation(FRotator::MakeFromEuler(FVector(RotationByCutDirection[noteData->Direction], 0, -90)));
	}

	UpdateNoteMaterial();
}

void ACNote::SetPreview()
{
	IsPreview = true;
}

bool ACNote::GetIsPreview()
{
	return IsPreview;
}
