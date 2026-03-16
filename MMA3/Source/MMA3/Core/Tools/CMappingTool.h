// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMappingTool.generated.h"

#define LEFT_NOTE_TOOL_ID FString("LeftNote")
#define RIGHT_NOTE_TOOL_ID FString("RightNote")
#define BOMB_TOOL_ID FString("Bomb")
#define WALL_TOOL FString("Wall")
#define DELETE_TOOL FString("Delete")
#define LEFT_EVENT_TOOL FString("LeftEvent")
#define RIGHT_EVENT_TOOL FString("RightEvent")

class ACMappingTool;

UCLASS()
class MMA3_API ACMappingTool : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACMappingTool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		FString ToolId;

	UPROPERTY()
		UStaticMeshComponent* ToolMeshComp;

	UPROPERTY()
		UStaticMesh* ToolMesh;

	UPROPERTY()
	UStaticMesh* ArrowStaticMesh;

	UPROPERTY()
		UMaterialInterface* ToolMaterial;

	UPROPERTY()
		FVector CustomPosition;

	UPROPERTY()
		FRotator CustomRotation;

	UPROPERTY()
		FVector CustomScale;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnUse(FVector position);

	void Update();

	virtual UStaticMesh* GetToolMesh();

	virtual UMaterialInterface* GetToolMaterial();

	virtual FVector GetCustomPosition();

	virtual FRotator GetCustomRotation();

	virtual FVector GetCustomScale();

};