// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "BeatCell.generated.h"

UCLASS()
class MMA3_API ABeatCell : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABeatCell();

	UPROPERTY(BlueprintReadOnly)
		UStaticMeshComponent* Plane;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetLength(float p_Length);

	UPROPERTY()
		UProceduralMeshComponent* Beats;

	UPROPERTY()
		UMaterialInstance* Material;

};
