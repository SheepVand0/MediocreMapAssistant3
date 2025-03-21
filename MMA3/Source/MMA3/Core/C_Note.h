// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMA3/Widgets/Structures.h"
#include "BeatCell.h"
#include "Kismet/GameplayStatics.h"
#include "MMA3/MMAConfig.h"
#include "C_Controller.h"
#include "ObjectFinder.h"
#include "C_Note.generated.h"

UCLASS()
class MMA3_API AC_Note : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AC_Note();

	~AC_Note();

	TMap<int, int> RotationByCutDirection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetNoteData(float p_Beat, int p_ColorType, int p_Line, int p_Layer, int p_Direction);

	void SetNoodleData(float p_Beat, int p_ColorType, int p_Line, int p_Layer, int p_Direction, FDefaultNoodleExtensionsData p_Data);

	UPROPERTY()
	float LastActorBeat;

	UPROPERTY()
	float ActorBeat;

	UPROPERTY()
	bool Binded;

	UFUNCTION()
		void OnTimeUpdated(float p_Time);

	UPROPERTY()
		UStaticMeshComponent* CubeMesh;

	UPROPERTY()
		UStaticMeshComponent* Arrow;

	UPROPERTY()
		bool IsFake;

	UPROPERTY()
		TArray<FVector2D> Coordinates;

	UPROPERTY()
		TArray<float> CoordinatesTimes;

	UPROPERTY()
		float Beat;

	UPROPERTY()
		int Line;

	UPROPERTY()
		int Layer;

	UPROPERTY()
		int Direction;

	UPROPERTY()
		int ColorType;
};