// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneGenerator.h"
#include "MMA3/Core/RenderWaveForm.h"

// Sets default values
APlaneGenerator::APlaneGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UMaterial>l_WaveformMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/Waveform/M_VertexColored.M_VertexColored'"));

	PlaneMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Plane Mesh"));
	PlaneMesh->SetMaterial(0, l_WaveformMaterial.Object);

	SetRootComponent(PlaneMesh);
}

// Called when the game starts or when spawned
void APlaneGenerator::BeginPlay()
{
	Super::BeginPlay();
	

}

void APlaneGenerator::OnConstruction(const FTransform& transform)
{
	//TArray<FVector> l_Vertices;
	//TArray<int> l_Triangles;
	//TArray<FLinearColor> l_VertexColors;

	//l_Vertices.Add(FVector(0, 0, 0));
	//l_Vertices.Add(FVector(0, 100, 0));

	//for (int l_i = 1; l_i < PlaneCount + 1; l_i++) {
	//	l_Vertices.Add(FVector(100 * l_i, 0, 0));
	//	l_Vertices.Add(FVector(100 * l_i, 100, 0));
	//}

	//for (int l_i = 0; l_i < PlaneCount; l_i++) {
	//	int l_TwoI = l_i * 2;
	//	l_Triangles.Add(l_TwoI + 1);
	//	l_Triangles.Add(l_TwoI + 2);
	//	l_Triangles.Add(l_TwoI);

	//	l_Triangles.Add(l_TwoI + 3);
	//	l_Triangles.Add(l_TwoI + 2);
	//	l_Triangles.Add(l_TwoI + 1);
	//}

	//for (int l_i = 0; l_i < l_Vertices.Num(); l_i++) {
	//	l_VertexColors.Add(FLinearColor(1, 0, 0));
	//}

	//PlaneMesh->CreateMeshSection_LinearColor(0, 
	//	l_Vertices, 
	//	l_Triangles, 
	//	TArray<FVector>(), 
	//	TArray<FVector2D>(), 
	//	l_VertexColors, 
	//	TArray<FProcMeshTangent>(), 
	//	false, 
	//	true);

	///*PlaneMesh->CreateMeshSection(0,
	//	l_Vertices,
	//	l_Triangles,
	//	TArray<FVector>(),
	//	TArray<FVector2D>(),
	//	TArray<FVector2D>(),
	//	TArray<FVector2D>(),
	//	TArray<FVector2D>(),
	//	TArray<FColor>(),
	//	TArray<FProcMeshTangent>(),
	//	false);*/
	
//	URenderSoundVisualization::GenerateVisMesh(PlaneMesh, 10, 10, 10, 10);
}

// Called every frame
void APlaneGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

