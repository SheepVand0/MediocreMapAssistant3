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

	virtual void OnUse(FVector p_Position) override;

};
