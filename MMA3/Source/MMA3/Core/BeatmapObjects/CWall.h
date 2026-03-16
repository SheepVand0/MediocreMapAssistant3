// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMA3/Core/Controller/CController.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "CWall.generated.h"

UCLASS()
class MMA3_API ACWall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACWall();

	static FVector CalculateObjectLocation(FWallData wallData);

	static FVector CalculateWallLocation(FWallData wallData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	TObjectPtr<UProceduralMeshComponent> WallMesh;

	FWallData* WallData;

	UPROPERTY()
	float MinBeat;

	UPROPERTY()
	float MaxBeat;

	void SetData(FWallData* wallData);

	void SetExtremumsBeats(float min, float max);

	void BuildMesh();

	void UpdateLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
