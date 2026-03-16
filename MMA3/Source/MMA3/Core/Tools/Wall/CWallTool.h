// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMA3/Core/Tools/CMappingTool.h"
#include "MMA3/Core/BeatmapObjects/CWall.h"
#include "CWallTool.generated.h"

/**
 * 
 */
UCLASS()
class MMA3_API ACWallTool : public ACMappingTool
{
	GENERATED_BODY()
	
public:

	ACWallTool();

	virtual void Tick(float deltaTime) override;

protected:

	TObjectPtr<ACWall> TempWall;

	bool IsPlacing;

	float LastBeat;

	void UpdateWallLocation();

public:

	virtual UStaticMesh* GetToolMesh() override;

	virtual UMaterialInterface* GetToolMaterial() override;

	virtual void OnUse(FVector location) override;

};
