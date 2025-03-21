// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Wall.h"

// Sets default values
AC_Wall::AC_Wall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UMaterial>l_ObstacleMat(TEXT("/Script/Engine.Material'/Game/Assets/Materials/Mapping/M_Wall.M_Wall'"));

	WallMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Wall Mesh"));
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

void AC_Wall::SetData(float beat, int type, int p_X, int width,  float length) {
	Beat = beat;
	Line = p_X;
	Width = width;
	Length = length;

	BuildMesh();
	SetActorLocation(FVector(0, beat, 0));
	//SetActorLocation(FVector(75 - (l_X * 25) - 50 + (25 / 2), p_Beat * 100, l_Y * 25 + (25 / 2)));
}

void AC_Wall::BuildMesh() {

	FVector l_ActorLocation = GetActorLocation();
	int l_Height = ((Type == 0) ? 3 : 2) * 50;

	float l_Width = Width * 50;
	float l_Length = Length * 50;

	TArray<FVector> l_Vertices;
	// Bottom
	l_Vertices.Add(FVector::Zero());			//Bottom left	    : 0
	l_Vertices.Add(FVector(l_Width, 0, 0));	    // Bottom right		: 1
	l_Vertices.Add(FVector(l_Width, l_Length, 0));  // Bottom far right : 2
	l_Vertices.Add(FVector(0, l_Length, 0));		// Bottom far left  : 3

	//Right Top
	l_Vertices.Add(FVector(l_Width, 0, l_Height));		// Top right	 : 4
	l_Vertices.Add(FVector(l_Width, l_Length, l_Height));	// Top far right : 5

	// Left Top
	l_Vertices.Add(FVector(0, 0, l_Height));		// Top left			 : 6
	l_Vertices.Add(FVector(0, l_Length, l_Height));	// Top far left		 : 7

	TArray<int32> l_Triangles;
	// Bottom
	l_Triangles.Add(0);
	l_Triangles.Add(1);
	l_Triangles.Add(3);
	l_Triangles.Add(1);
	l_Triangles.Add(2);
	l_Triangles.Add(3);
	// Front
	l_Triangles.Add(0);
	l_Triangles.Add(1);
	l_Triangles.Add(6);
	l_Triangles.Add(1);
	l_Triangles.Add(4);
	l_Triangles.Add(6);
	// Right
	l_Triangles.Add(1);
	l_Triangles.Add(2);
	l_Triangles.Add(4);
	l_Triangles.Add(2);
	l_Triangles.Add(5);
	l_Triangles.Add(4);
	// Left
	l_Triangles.Add(0);
	l_Triangles.Add(6);
	l_Triangles.Add(7);
	l_Triangles.Add(0);
	l_Triangles.Add(3);
	l_Triangles.Add(7);
	// Top
	l_Triangles.Add(6);
	l_Triangles.Add(7);
	l_Triangles.Add(5);
	l_Triangles.Add(6);
	l_Triangles.Add(4);
	l_Triangles.Add(5);

	TArray<FVector> l_Normals;
	l_Normals.Init(FVector(0.f, 0.f, 1.f), l_Vertices.Num());

	TArray<FProcMeshTangent> l_Tangents;
	l_Tangents.Init(FProcMeshTangent(0, -1, 0), 4);

	TArray<FVector2D> l_UV0;


	WallMesh->CreateMeshSection(0, l_Vertices, l_Triangles, l_Normals, l_UV0, TArray<FColor>(), l_Tangents, false);
	WallMesh->bUseComplexAsSimpleCollision = true;
	//Beats->SetMaterial(0, Material);
}