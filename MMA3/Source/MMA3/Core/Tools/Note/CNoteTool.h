// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMA3/Core/Tools/CMappingTool.h"
#include "MMA3/Core/Controller/CController.h"
#include "MMA3/Core/CObjectsSpawner.h"
#include "CNoteTool.generated.h"

UCLASS()
class MMA3_API ACNoteTool : public ACMappingTool {

	GENERATED_BODY()

protected:
	
	virtual void OnUpdate(const FVector& position) override;
	
public:

	ACNoteTool();

	virtual void Tick(float delta) override;

	UPROPERTY() UStaticMeshComponent* ArrowMesh;
	UPROPERTY() UStaticMesh* DotMesh;
	UPROPERTY() TObjectPtr<UMaterialInterface> NoteMaterial;
	UPROPERTY() int Type;
	UPROPERTY() int Direction;

	virtual void OnFinishUsing(FVector p_Position) override;

	virtual UStaticMesh* GetToolMesh() override;

	virtual UMaterialInterface* GetToolMaterial() override;

	virtual FVector GetCustomScale() override;
};
