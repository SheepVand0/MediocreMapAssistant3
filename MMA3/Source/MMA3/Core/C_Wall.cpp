// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Wall.h"

// Sets default values
AC_Wall::AC_Wall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>l_Cube(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/Wall.Wall'"));
	ConstructorHelpers::FObjectFinder<UMaterial>l_ObstacleMat(TEXT("/Script/Engine.Material'/Game/Assets/Materials/Mapping/M_Obstacle.M_Obstacle'"));

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall Mesh"));
	WallMesh->SetStaticMesh(l_Cube.Object);
	WallMesh->SetMaterial(0, l_ObstacleMat.Object);
}

// Called when the game starts or when spawned
void AC_Wall::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AC_Wall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_Wall::SetData(float p_Beat, int p_Type, int p_X, int p_Width,  float p_Length) {

	int l_Height = p_Type == 0 ? 2 : 1;
	int l_Y = p_Type == 0 ? 0 : 1;
	int l_X = p_X;
	if (l_X + Width > 3)
		l_X -= Width;

	SetActorLocation(FVector(75 - (l_X * 25) - 50 + (25 / 2), p_Beat * 100, l_Y * 25 + (25 / 2)));

	WallMesh->SetRelativeScale3D(FVector((float)p_Width / 2, p_Length / (AC_Controller::Instance->MapData.BPM / 60), l_Height / 4));
}