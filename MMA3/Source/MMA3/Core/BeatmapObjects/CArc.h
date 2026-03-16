// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMA3/Core/Structures.h"
#include "MMA3/Core/Controller/CController.h"
#include "CArc.generated.h"

UCLASS()
class MMA3_API ACArc : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACArc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static FVector CalculateObjectLocation(FArcData arcData);

	static FVector CalculateArcLocation(FArcData arcData);

	static FVector CalculateArcTailLocation(FArcData arcData);

	TObjectPtr<UMaterialInterface> OriginalMaterial;

	TObjectPtr<UProceduralMeshComponent> ArcMesh;

	FArcData ArcData;

	UPROPERTY()
	int Subdivisions;

	void SetData(FArcData arc);

	UFUNCTION()
	void UpdateMaterial();

	UFUNCTION()
	void BuildArcMesh();

};
