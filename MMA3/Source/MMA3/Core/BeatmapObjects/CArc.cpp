// Fill out your copyright notice in the Description page of Project Settings.


#include "CArc.h"
#include "CNote.h"
#include "KismetProceduralMeshLibrary.h"


// Sets default values
ACArc::ACArc()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UMaterial>l_Mat(TEXT("/Script/Engine.Material'/Game/Assets/Materials/M_Arc.M_Arc'"));
	OriginalMaterial = l_Mat.Object;

	ArcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Arc Mesh"));
	SetRootComponent(ArcMesh);

	Subdivisions = 10;
}

// Called when the game starts or when spawned
void ACArc::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACArc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ACArc::CalculateObjectLocation(FArcData arcData)
{
	return CalculateArcLocation(arcData);
}

FVector ACArc::CalculateArcLocation(FArcData arcData)
{
	return FVector(75 - (arcData.Line * 25) + (25.f / 2), arcData.Beat * 100, arcData.Layer * 25 + (25.f / 2));
}

FVector ACArc::CalculateArcTailLocation(FArcData arcData)
{
	return FVector(75 - (arcData.TailLine * 25) + (25.f / 2), arcData.TailBeat * 100 * GetConfig()->MappingGridScale, arcData.TailLayer * 25 + (25.f / 2));
}

void ACArc::SetData(FArcData arc)
{
	ArcData = arc;

	SetActorRelativeLocation(CalculateArcLocation(arc));

	BuildArcMesh();
	UpdateMaterial();
}

void ACArc::UpdateMaterial()
{
	UMaterialInstance* l_Mat = GetMappingController()->UpdateNoteMaterial(OriginalMaterial, ArcData.Color, false, false);
	ArcMesh->SetMaterial(0, l_Mat);
}

void ACArc::BuildArcMesh()
{
	if (Subdivisions == 0) {
		return;
	}

	const int l_VerticesCount = 2 * Subdivisions;
	const float l_Width = 5;

	float l_HeadRad = FMath::DegreesToRadians(ACNote::RotationByCutDirection[ArcData.HeadDirection]);
	float l_TailRad = FMath::DegreesToRadians(ACNote::RotationByCutDirection[ArcData.TailDirection]);

	const FVector l_HeadVec = FVector(cos(l_HeadRad - (PI / 2)), 0, sin(l_HeadRad - (PI / 2))) * 20;
	const FVector l_TailVec = FVector(cos(l_TailRad - (PI / 2)), 0, sin(l_TailRad - (PI / 2))) * 20;

	UE_LOG(LogTemp, Display, TEXT("Head Vec: %lf %lf %lf"), l_HeadVec.X, l_HeadVec.Y, l_HeadVec.Z);
	UE_LOG(LogTemp, Display, TEXT("Tail Vec: %lf %lf %lf"), l_TailVec.X, l_TailVec.Y, l_TailVec.Z);

	TArray<FVector> l_Vertices0;
	TArray<FVector> l_Vertices1;

	l_Vertices0.AddDefaulted(l_VerticesCount);
	l_Vertices1.AddDefaulted(l_VerticesCount);

	TArray<FVector2D> l_UVs0;
	TArray<FVector2D> l_UVs1;

	l_UVs0.AddDefaulted(l_VerticesCount);
	l_UVs1.AddDefaulted(l_VerticesCount);

	TArray<FLinearColor> l_VertColors0;
	TArray<FLinearColor> l_VertColors1;

	l_VertColors0.AddZeroed(l_VerticesCount);
	l_VertColors1.AddZeroed(l_VerticesCount);

	TArray<int> l_Triangles0;
	TArray<int> l_Triangles1;

	/////////////////////////////////////////////////

	//l_Vertices1[0] = FVector(0, -l_Width, 0);
	//l_Vertices1[1] = FVector(0, l_Width, 0);

	FVector l_ArcLocation = CalculateArcLocation(ArcData);
	FVector l_TailLocation = CalculateArcTailLocation(ArcData);

	l_ArcLocation.Y = 0;
	l_TailLocation.Y = ((ArcData.TailBeat - ArcData.Beat) * 100 * GetConfig()->MappingGridScale);

	FVector l_HeadPoint = l_ArcLocation - l_HeadVec;
	l_HeadPoint.Y = 0;

	FVector l_TailPoint = l_TailLocation - l_TailVec;
	l_TailPoint.Y = (ArcData.TailBeat - ArcData.Beat) * 100 * GetConfig()->MappingGridScale;

	FVector l_MidPoint = (l_TailPoint + l_HeadPoint) / 2 + (l_HeadVec - l_TailVec);
	l_MidPoint.Y = l_TailPoint.Y / 2;

	FVectorAnimation l_PointsTransition = FVectorAnimation({
		new FVectorAnimationKey(0, l_ArcLocation),	
		//new FVectorAnimationKey(Subdivisions * 0.1f, l_HeadPoint),
		new FVectorAnimationKey((Subdivisions - 1) / 2.f, l_MidPoint),
		//new FVectorAnimationKey(Subdivisions * 0.9f, l_TailPoint),
		new FVectorAnimationKey(Subdivisions - 1, l_TailLocation)
	}, nullptr, nullptr);

	FVector l_VertZero = l_PointsTransition.Evaluate(0);
	l_Vertices0[0] = l_VertZero + FVector(-l_Width, 0, 0);
	l_Vertices0[1] = l_VertZero + FVector(l_Width, 0, 0);

	for (int l_i = 1; l_i < Subdivisions; l_i++) {
		int l_VertIndex = l_i * 2;

		FVector l_AnimationEvaluation = l_PointsTransition.Evaluate(l_i);

		l_Vertices0[l_VertIndex]     = l_AnimationEvaluation + FVector(-l_Width, 0, 0);
		l_Vertices0[l_VertIndex + 1] = l_AnimationEvaluation + FVector(l_Width, 0, 0);

		UKismetProceduralMeshLibrary::ConvertQuadToTriangles(l_Triangles0, l_VertIndex - 2, l_VertIndex - 1, l_VertIndex + 1, l_VertIndex);
	}

	ArcMesh->CreateMeshSection_LinearColor(0, l_Vertices0, l_Triangles0, TArray<FVector>(), l_UVs0, l_VertColors0, TArray<FProcMeshTangent>(), false);
}



