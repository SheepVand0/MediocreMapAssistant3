// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelSelectionWidget.generated.h"

/**
 *
 */
UCLASS(Abstract)
class MMA3_API ULevelSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCanvasPanel* MainCanvas;

};
