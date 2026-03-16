// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "MapButton.generated.h"



/**
 * Button used for map selection
 */
UCLASS()
class MMA3_API UMapButton : public UButton
{
	GENERATED_BODY()

public:

	UMapButton();

	UPROPERTY(BlueprintReadOnly)
		FSlateBrush m_NormalBrush;

	UPROPERTY(BlueprintReadOnly)
		FSlateBrush m_PressedBrush;
};
