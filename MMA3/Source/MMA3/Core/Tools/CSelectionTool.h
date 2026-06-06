// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMA3/Core/Tools/CMappingTool.h"
#include "CSelectionTool.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedSelectionDelegate);

class UProceduralMeshComponent;

/**
 * 
 */
UCLASS()
class MMA3_API ACSelectionTool : public ACMappingTool
{
	GENERATED_BODY()
	
protected:
	
	TObjectPtr<UMaterialInstance> ProceduralMeshMaterial;
	
	UPROPERTY(BlueprintReadOnly) float StartTime;
	UPROPERTY(BlueprintReadOnly) float EndTime;
	
	int StartX;
	int StartZ;
	
public:
	
	ACSelectionTool();
	
	UPROPERTY() TObjectPtr<UProceduralMeshComponent> SelectionBox;
	UPROPERTY(EditAnywhere, BlueprintAssignable) FOnFinishedSelectionDelegate OnFinishedSelection; 
	
	virtual void OnFinishUsing(FVector position) override;
	
	virtual void OnStartUsing(FVector position) override;
	
	virtual void OnUpdateUse(const FVector& position) override;
};
