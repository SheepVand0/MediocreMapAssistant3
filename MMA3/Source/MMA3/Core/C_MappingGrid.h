// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_MappingGrid.generated.h"

UCLASS()
class MMA3_API AC_MappingGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_MappingGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
