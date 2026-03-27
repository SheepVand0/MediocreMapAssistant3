// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMA3/Core/Structures.h"
#include "MMA3/Core/Mapping/BeatCell.h"
#include "Kismet/GameplayStatics.h"
#include "MMA3/MMAConfig.h"
#include "MMA3/Core/Controller/CController.h"
#include "MMA3/MMA3.h"
#include "CNote.generated.h"

UCLASS()
class MMA3_API ACNote : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACNote();

	~ACNote();

	static TMap<int, int> RotationByCutDirection;

	static FVector CalculateObjectLocation(FNoteData noteData);

	static FVector CalculateNoteLocation(FNoteData noteData);

	static void InitAngles();

	static int CutDirectionFromAngle(int angle);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool Passed;

	bool IsPreview;

	void UpdateNoteMaterial(bool isSelected = false);

	TObjectPtr<UMaterialInterface> NoteMaterial;

	TObjectPtr<UMaterialInterface> BombMaterial;

	TObjectPtr<UMaterialInterface> OutlineMaterial;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FNoteData* NoteData;

	UPROPERTY()
	UStaticMeshComponent* CubeMesh;

	UPROPERTY()
	UStaticMeshComponent* Arrow;

	float LastActorBeat;

	float ActorBeat;

	void SetData(FNoteData* noteData);

	void SetPreview();

	bool GetIsPreview();
};