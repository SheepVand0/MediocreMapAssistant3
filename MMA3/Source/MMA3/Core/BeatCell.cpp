// Fill out your copyright notice in the Description page of Project Settings.

#include "BeatCell.h"
#include "ObjectFinder.h"

// Sets default values
ABeatCell::ABeatCell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UMaterialInstance>l_Mat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/Mapping/M_BpmGrid.M_BpmGrid'"));
	Material = l_Mat.Object;

	Beats = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Beats"));
	Beats->ComponentTags.Add("MappingGrid");

	Tags.Add("MappingGrid");
}

// Called when the game starts or when spawned
void ABeatCell::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABeatCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABeatCell::SetLength(float p_Length) {
	TArray<FVector> l_Vertices;
	l_Vertices.Add(FVector(-50, 0, 0));
	l_Vertices.Add(FVector(-50, p_Length * 100, 0));
	l_Vertices.Add(FVector(50, p_Length * 100, 0));
	l_Vertices.Add(FVector(50, 0, 0));

	TArray<int32> l_Triangles;
	l_Triangles.Add(0);
	l_Triangles.Add(1);
	l_Triangles.Add(2);
	l_Triangles.Add(0);
	l_Triangles.Add(2);
	l_Triangles.Add(3);

	TArray<FVector> l_Normals;
	l_Normals.Init(FVector(0.f, 0.f, 1.f), 4);

	TArray<FVector2D> l_UV0;
	l_UV0.Add(FVector2D(0, 0));
	l_UV0.Add(FVector2D(0, p_Length));
	l_UV0.Add(FVector2D(1, p_Length));
	l_UV0.Add(FVector2D(1, 0));

	TArray<FProcMeshTangent> l_Tangents;
	l_Tangents.Init(FProcMeshTangent(0, -1, 0), 4);

	Beats->CreateMeshSection_LinearColor(0, l_Vertices, l_Triangles, l_Normals, l_UV0, TArray<FLinearColor>(), l_Tangents, true);
	Beats->bUseComplexAsSimpleCollision = true;
	Beats->SetMaterial(0, Material);
}