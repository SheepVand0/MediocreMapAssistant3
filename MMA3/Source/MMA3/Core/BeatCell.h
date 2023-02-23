// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C:/UE/UE5_1_1/UE_5.1/Engine/Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
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

	UProceduralMeshComponent* Beats;

	UMaterialInstance* Material;

};
