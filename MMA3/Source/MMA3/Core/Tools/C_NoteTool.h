// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMA3/Core/C_MappingTool.h"
#include "C_NoteTool.generated.h"

/**
 *
 */
UCLASS()
class MMA3_API AC_NoteTool : public AC_MappingTool
{
	GENERATED_BODY()

public:

	AC_NoteTool();

	virtual void OnUse(FVector p_Position) override;

};

UCLASS()
class MMA3_API USubNoteTool : public UMappingSubTool {

	GENERATED_BODY()

public:

	USubNoteTool();

	UPROPERTY()
		int Type;

	UPROPERTY()
		UStaticMesh* NoteToolMesh;

	UPROPERTY()
		UMaterialInterface* NoteMaterial;

	UPROPERTY()
		UMaterialInterface* BombMaterial;

	UPROPERTY()
		UStaticMesh* BombToolMesh;

	virtual void OnUse(FVector p_Position) override;

	virtual UStaticMesh* GetToolMesh() override;

	virtual UMaterialInterface* GetToolMaterial() override;
};
