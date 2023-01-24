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
		UMapButton* CButton;

	UPROPERTY(meta = (BindWidget))
		UCustomTextBlock* CName;

	UPROPERTY(meta = (BindWidget))
		UCustomTextBlock* CAuthor;

	UPROPERTY(meta = (BindWidget))
		UCustomTextBlock* CMapper;

	UPROPERTY(meta = (BindWidget))
		UCustomTextBlock* CBpm;

	UPROPERTY(meta = (BindWidget))
		UCustomTextBlock* CDuration;

private:

	UPROPERTY()
		FMapInfo m_Info;

public:

	UFUNCTION()
		void SetData(FMapInfo p_MapInfo);

	UFUNCTION()
		FMapInfo GetData();


};
