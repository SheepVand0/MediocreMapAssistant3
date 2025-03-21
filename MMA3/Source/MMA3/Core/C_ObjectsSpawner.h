// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "C_Note.h"
#include "C_Wall.h"
#include "MMA3/Core/C_Controller.h"
#include "C_ObjectsSpawner.generated.h"

UCLASS()
class MMA3_API AC_ObjectsSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AC_ObjectsSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SpawnObjects(FMapData map, ABeatCell* beatCell);

};