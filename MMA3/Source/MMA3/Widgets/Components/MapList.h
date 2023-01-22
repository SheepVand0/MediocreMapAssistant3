// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomScrollBox.h"
#include "MMA3/Widgets/Structures.h"
#include "MapList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMapButtonPressed);

/**
 *
 */
UCLASS()
class MMA3_API UMapList : public UCustomScrollBox
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FOnMapButtonPressed OnMapButtonPressedEvent;

};
