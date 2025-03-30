// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PlaneGenerator.generated.h"

UCLASS()
class MMA3_API APlaneGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaneGenerator();

	UProceduralMeshComponent* PlaneMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int PlaneCount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
