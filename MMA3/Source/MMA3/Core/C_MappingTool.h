// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_MappingTool.generated.h"

#define LEFT_NOTE_TOOL_ID FString("LeftNote")
#define RIGHT_NOTE_TOOL_ID FString("RightNote")
#define BOMB_TOOL_ID FString("Bomb")
#define WALL_TOOL FString("Wall")
#define DELETE_TOOL FString("Delete")
#define LEFT_EVENT_TOOL FString("LeftEvent")
#define RIGHT_EVENT_TOOL FString("RightEvent")

class AC_MappingTool;
class UMappingSubTool;

UCLASS()
class MMA3_API AC_MappingTool : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AC_MappingTool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		FString ToolId;

	UPROPERTY()
		UStaticMeshComponent* ToolMeshComp;

	UPROPERTY()
		TArray<UMappingSubTool*> SubTools;

	UPROPERTY()
		UMappingSubTool* CurrentSubTool;

	UPROPERTY()
		int SelectedSubToolIndex;

public:

	UFUNCTION()
		void SelectSubTool(int p_Index);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnUse(FVector p_Position);

	UMappingSubTool* GetSelectedSubTool();

	void Update();

};

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

UCLASS()
class MMA3_API UMappingSubTool : public USceneComponent {

	GENERATED_BODY()

protected:

	UPROPERTY()
		UStaticMesh* ToolMesh;

	UPROPERTY()
		UMaterialInterface* ToolMaterial;

	UPROPERTY()
		FVector CustomPosition;

	UPROPERTY()
		FRotator CustomRotation;

	UPROPERTY()
		FVector CustomScale;

public:

	virtual UStaticMesh* GetToolMesh();

	virtual UMaterialInterface* GetToolMaterial();

	virtual void OnUse(FVector p_Position);

	virtual FVector GetCustomPosition();

	virtual FRotator GetCustomRotation();

	virtual FVector GetCustomScale();

};
