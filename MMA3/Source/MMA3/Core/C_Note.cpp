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
	Arrow->SetRelativeScale3D(FVector(4, 1, 1));

	SetActorScale3D(FVector(0.22f, 0.22f, 0.22f));
}

// Called when the game starts or when spawned
void AC_Note::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AC_Note::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_Note::SetNoteData(float p_Beat, int p_ColorType, int p_Line, int p_Layer, int p_Direction) {
	Beat = p_Beat;
	ColorType = p_ColorType;
	Line = p_Layer;
	Layer = p_Layer;
	Direction = p_Direction;
	SetActorLocation(FVector(Line, Beat, Layer));
}

void AC_Note::SetNoodleData(float p_Beat, int p_ColorType, int p_Line, int p_Layer, int p_Direction, FDefaultNoodleExtensionsData p_Data) {
	SetNoteData(p_Beat, p_ColorType, p_Line, p_Layer, p_Direction);
}
