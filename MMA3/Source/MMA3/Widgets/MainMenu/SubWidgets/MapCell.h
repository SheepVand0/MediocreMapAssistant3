// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MMA3/Widgets/Components/MapButton.h"
#include "MMA3/Widgets/Components/CustomTextBlock.h"
#include "MMA3/Widgets/Structures.h"
#include "MapCell.generated.h"

/**
 *
 */
UCLASS()
class MMA3_API UMapCell : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
		UMapButton* m_Button;

	UPROPERTY(meta = (BindWidget))
		UCustomTextBlock* m_Name;

	UPROPERTY(meta = (BindWidget))
		UCustomTextBlock* m_Author;

	UPROPERTY(meta = (BindWidget))
		UCustomTextBlock* m_Mapper;

private:

	UPROPERTY()
		FMapInfo m_Info;

public:

	UFUNCTION()
		void SetData(FMapInfo p_MapInfo);

	UFUNCTION()
		FMapInfo GetData();


};
