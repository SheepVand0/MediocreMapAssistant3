// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_Controller.h"
#include "ProceduralMeshComponent.h"
#include "C_Wall.generated.h"

UCLASS()
class MMA3_API AC_Wall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AC_Wall();

	UPROPERTY()
		UProceduralMeshComponent* WallMesh;

	UPROPERTY()
		float Beat;

	UPROPERTY()
		int Line;

	UPROPERTY()
		int Type;

	UPROPERTY()
		int Width;

	UPROPERTY()
		int Height;

	UPROPERTY()
		float Length;

	void SetData(float beat, int type, int x, int width, float length);

	void BuildMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
