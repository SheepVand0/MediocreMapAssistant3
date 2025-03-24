// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Note.h"

// Sets default values
AC_Note::AC_Note()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UMaterialInstance>l_CubeMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/Mapping/M_NoteInstance.M_NoteInstance'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>l_Cube(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/NoteBody_Cube.NoteBody_Cube'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>l_Arrow(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/NoteBody_Cylinder.NoteBody_Cylinder'"));

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	SetRootComponent(CubeMesh);
	CubeMesh->SetStaticMesh(l_Cube.Object);
	CubeMesh->SetMaterial(0, l_CubeMat.Object);

	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(CubeMesh);
	Arrow->SetStaticMesh(l_Arrow.Object);
	Arrow->SetRelativeScale3D(FVector(1, 1, 1));

	SetActorScale3D(FVector(0.17f, 0.17f, 0.17f));

	if (RotationByCutDirection.Num() == 0) {
		RotationByCutDirection.Add(0);
		RotationByCutDirection.Add(1);
		RotationByCutDirection.Add(2);
		RotationByCutDirection.Add(3);
		RotationByCutDirection.Add(4);
		RotationByCutDirection.Add(5);
		RotationByCutDirection.Add(6);
		RotationByCutDirection.Add(7);
		RotationByCutDirection[0] = 180;
		RotationByCutDirection[1] = 0;
		RotationByCutDirection[2] = 270;
		RotationByCutDirection[3] = 90;
		RotationByCutDirection[4] = 225;
		RotationByCutDirection[5] = 135;
		RotationByCutDirection[6] = 315;
		RotationByCutDirection[7] = 45;
	}
}

AC_Note::~AC_Note() {
}

// Called when the game starts or when spawned
void AC_Note::BeginPlay()
{
	Super::BeginPlay();

	AC_Controller::Instance->OnTimeUpdated.AddDynamic(this, &AC_Note::OnTimeUpdated);
}

// Called every frame
void AC_Note::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!AC_Controller::Instance) {
		UE_LOG(LogExit, Error, TEXT("Controller Instance is null"));

		exit(-1);
	}

	ActorBeat = AC_Controller::Instance->GetBeat();
	
	if (ActorBeat > LastActorBeat) {
		if ((ActorBeat > Beat) && (LastActorBeat < Beat)) {
			UGameplayStatics::PlaySound2D(GetWorld(), AC_Controller::Instance->HitSound, 0.8f);
		}
	}

	if (Beat < AC_Controller::Instance->GetBeat() && ColorType != 3) {
		//if (!(CubeMesh->GetMaterial(0)->GetName()[0] != 'P')) {
			if (ColorType == 0)
				CubeMesh->SetMaterial(0, AC_Controller::Instance->PassedLeftNoteMaterial);
			else if (ColorType == 1)
				CubeMesh->SetMaterial(0, AC_Controller::Instance->PassedRightNoteMaterial);
		//}
	}
	else {
		//if (!(CubeMesh->GetMaterial(0)->GetName()[0] != 'N')) {
			if (ColorType == 0)
				CubeMesh->SetMaterial(0, AC_Controller::Instance->LeftNoteMaterial);
			else if (ColorType == 1)
				CubeMesh->SetMaterial(0, AC_Controller::Instance->RightNoteMaterial);
		//}
	}

	if (!this) {
		UE_LOG(LogExit, Error, TEXT("This is null (AC_Note)"));
		exit(-1);
	}

	LastActorBeat = ActorBeat;
}

void AC_Note::SetNoteData(float p_Beat, int p_ColorType, int p_Line, int p_Layer, int p_Direction) {
	Beat = p_Beat;
	ColorType = p_ColorType;
	Line = p_Line;
	Layer = p_Layer;
	Direction = p_Direction;
	SetActorLocation(FVector(75 - (Line * 25) - 50 + (25/2), Beat * 100, Layer * 25 + (25/2)));
	if (ColorType == 3) {
		Arrow->SetVisibility(false);
		CubeMesh->SetStaticMesh(AC_Controller::Instance->BombMesh);
		CubeMesh->SetRelativeScale3D(FVector(-0.08f, -0.08f, -0.08f));
		CubeMesh->SetMaterial(0, AC_Controller::Instance->BombMaterial);
	}
	else if (p_Direction == 8) {
		Arrow->SetStaticMesh(AC_Controller::Instance->DotMesh);
		SetActorRotation(FRotator::MakeFromEuler(FVector(0, 0, -90)));
	}
	else {
		SetActorRotation(FRotator::MakeFromEuler(FVector(RotationByCutDirection[Direction], 0, -90)));
	}
	/*if (UGameplayStatics::DoesSaveGameExist(MMA_SAVE_GAME_SLOT_NAME, 0) && ColorType != 3) {

		if (ColorType == 0) {
			CubeMesh->SetMaterial(0, AC_Controller::Instance->LeftNoteMaterial);
		}
		else if (ColorType == 1) {
			
		}
		
	}*/
}

void AC_Note::SetNoodleData(float p_Beat, int p_ColorType, int p_Line, int p_Layer, int p_Direction, FDefaultNoodleExtensionsData p_Data) {
	SetNoteData(p_Beat, p_ColorType, p_Line, p_Layer, p_Direction);
}

void AC_Note::OnTimeUpdated(float p_Time) {

	/*if (p_Time < Beat && p_Time > Beat - 0.05f && !PlayedSound) {
		PlayedSound = true;
		if (AC_Controller::Instance->Playing) {
			
		}
	}*/
}