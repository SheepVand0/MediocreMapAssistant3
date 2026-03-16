// Fill out your copyright notice in the Description page of Project Settings.


#include "CWall.h"

// Sets default values
ACWall::ACWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UMaterialInstance>l_ObstacleMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/M_Obstacle_Inst.M_Obstacle_Inst'"));

	WallMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Wall Mesh"));
	WallMesh->SetMaterial(0, l_ObstacleMat.Object);
	SetRootComponent(WallMesh);
	WallMesh->SkipUpdateOverlapsOptimEnabled = true;
	WallMesh->bUseComplexAsSimpleCollision = true;
}

FVector ACWall::CalculateObjectLocation(FWallData wallData)
{
	return CalculateWallLocation(wallData);
}

FVector ACWall::CalculateWallLocation(FWallData wallData)
{
		return FVector((wallData.Line * 25 + 25), wallData.Beat * GetConfig()->MappingGridScale * 100, wallData.Layer * 25);
	}

// Called when the game starts or when spawned
void ACWall::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACWall::SetData(FWallData* wallData) {
	WallData = wallData;

	UpdateLocation();
}

void ACWall::SetExtremumsBeats(float min, float max)
{
	MinBeat = min;
	MaxBeat = max;
}

void ACWall::BuildMesh() {

	//UE_LOG(LogTemp, Display, TEXT("%f %d %d"), WallData->Beat, WallData->Width, WallData->Height);

	//FVector l_ActorLocation = GetActorLocation();
	int l_Height = (int)WallData->Height * (75.f / 3);
	float l_Width = (int)WallData->Width * 25;
	float l_Length = WallData->Duration * 100;

	float l_Start = 0;

	float l_EndBeat = (WallData->Beat + WallData->Duration);

	bool l_StartLimited = false;
	bool l_EndLimited = false;

	if (MaxBeat < l_EndBeat) {
		l_Length = (MaxBeat - WallData->Beat) * 100;
		l_EndLimited = true;
	}

	if (WallData->Beat < MinBeat) {
		l_Start = (MinBeat - WallData->Beat) * 100;
		l_StartLimited = true;
	}

	FVector l_Verts[8];
	TArray<FVector> l_Normals;
	TArray<FVector> l_Vertices;
	TArray<int32> l_Triangles;
	TArray<FVector2D> l_UVs;

	l_Vertices.AddUninitialized(6 * 4);
	l_Normals.AddUninitialized(6 * 4);
	l_UVs.AddUninitialized(6 * 4);

	// One way
	/*l_Verts[0] = FVector(0, l_Start, 0);			    // Bottom right	     : 0
	l_Verts[1] = FVector(l_Width, l_Start, 0);	        // Bottom left		 : 1
	l_Verts[2] = FVector(l_Width, l_Length, 0);         // Bottom far left   : 2
	l_Verts[3] = FVector(0, l_Length, 0);		        // Bottom far right  : 3
	l_Verts[4] = FVector(l_Width, l_Start, l_Height);	// Top left	         : 4
	l_Verts[5] = FVector(l_Width, l_Length, l_Height);	// Top far left      : 5
	l_Verts[6] = FVector(0, l_Start, l_Height);		    // Top right		 : 6
	l_Verts[7] = FVector(0, l_Length, l_Height);	    // Top far right	 : 7*/

	// The other way
	l_Verts[0] = FVector(-l_Width, l_Start, 0);			// Bottom right	     : 0
	l_Verts[1] = FVector(0, l_Start, 0);			    // Bottom left		 : 1
	l_Verts[2] = FVector(0, l_Length, 0);				// Bottom far left   : 2
	l_Verts[3] = FVector(-l_Width, l_Length, 0);		// Bottom far right  : 3

	l_Verts[4] = FVector(0, l_Start, l_Height);	            // Top left	         : 4
	l_Verts[5] = FVector(0, l_Length, l_Height);	        // Top far left      : 5
	l_Verts[6] = FVector(-l_Width, l_Start, l_Height);		// Top right		 : 6
	l_Verts[7] = FVector(-l_Width, l_Length, l_Height);	    // Top far right	 : 7

	// Front Face
	l_Vertices[0] = l_Verts[0]; // br
	l_Vertices[1] = l_Verts[1]; // bl
	l_Vertices[2] = l_Verts[6]; // tr
	l_Vertices[3] = l_Verts[4]; // tl
	if (!l_StartLimited) {
		UKismetProceduralMeshLibrary::ConvertQuadToTriangles(l_Triangles, 2, 3, 1, 0);
	}
	l_Normals[0] = l_Normals[1] = l_Normals[2] = l_Normals[3] = FVector(0, -1, 0);

	// Right Face
	l_Vertices[4] = l_Verts[0]; // br
	l_Vertices[5] = l_Verts[3]; // bfr
	l_Vertices[6] = l_Verts[6]; // tr
	l_Vertices[7] = l_Verts[7]; // tfr
	UKismetProceduralMeshLibrary::ConvertQuadToTriangles(l_Triangles, 4, 5, 7, 6);
	l_Normals[4] = l_Normals[5] = l_Normals[6] = l_Normals[7] = FVector(-1, 0, 0);

	// Left Face
	l_Vertices[8]  = l_Verts[1]; // bl
	l_Vertices[9]  = l_Verts[2]; // bfl
	l_Vertices[10] = l_Verts[4]; // tl
	l_Vertices[11] = l_Verts[5]; // tfl
	UKismetProceduralMeshLibrary::ConvertQuadToTriangles(l_Triangles, 10, 11, 9, 8);
	l_Normals[8] = l_Normals[9] = l_Normals[10] = l_Normals[11] = FVector(1, 0, 0);

	// Back Face
	l_Vertices[12] = l_Verts[2]; // bfl
	l_Vertices[13] = l_Verts[3]; // bfr
	l_Vertices[14] = l_Verts[5]; // tfl
	l_Vertices[15] = l_Verts[7]; // tfr
	if (!l_EndLimited) {
		UKismetProceduralMeshLibrary::ConvertQuadToTriangles(l_Triangles, 13, 12, 14, 15);
	}
	l_Normals[12] = l_Normals[13] = l_Normals[14] = l_Normals[15] = FVector(0, 1, 0);

	// Bottom Face
	l_Vertices[16] = l_Verts[1]; // bl
	l_Vertices[17] = l_Verts[0]; // br
	l_Vertices[18] = l_Verts[2]; // bfl
	l_Vertices[19] = l_Verts[3]; // bfr
	UKismetProceduralMeshLibrary::ConvertQuadToTriangles(l_Triangles, 16, 17, 19, 18);
	l_Normals[16] = l_Normals[17] = l_Normals[18] = l_Normals[19] = FVector(0, 0, -1);

	// Top Face
	l_Vertices[20] = l_Verts[4]; // tl
	l_Vertices[21] = l_Verts[6]; // tr
	l_Vertices[22] = l_Verts[5]; // tfl
	l_Vertices[23] = l_Verts[7]; // tfr
	UKismetProceduralMeshLibrary::ConvertQuadToTriangles(l_Triangles, 20, 21, 23, 22);
	l_Normals[20] = l_Normals[21] = l_Normals[22] = l_Normals[23] = FVector(0, 1, 0);

	l_UVs[0] = l_UVs[4] = l_UVs[8] = l_UVs[12] = l_UVs[16] = l_UVs[20] = FVector2D(0.f, 0.f);
	l_UVs[1] = l_UVs[5] = l_UVs[9] = l_UVs[13] = l_UVs[17] = l_UVs[21] = FVector2D(1.f, 0.f);
	l_UVs[2] = l_UVs[6] = l_UVs[10] = l_UVs[14] = l_UVs[18] = l_UVs[22] = FVector2D(0.f, 1.f);
	l_UVs[3] = l_UVs[7] = l_UVs[11] = l_UVs[15] = l_UVs[19] = l_UVs[23] = FVector2D(1.f, 1.f);

	///////////////////////////////////////////////////////////////////////////////////////////////

	TArray<FProcMeshTangent> l_Tangents;
	//l_Tangents.Init(FProcMeshTangent(0, -1, 0), 4);

	TArray<FVector2D> l_UV0;

	WallMesh->CreateMeshSection(0, l_Vertices, l_Triangles, l_Normals, l_UVs, TArray<FColor>(), l_Tangents, false);
}

void ACWall::UpdateLocation()
{
	SetActorRelativeLocation(CalculateWallLocation(*WallData));
}
