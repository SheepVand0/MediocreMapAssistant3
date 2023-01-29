// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomScrollBox.h"
#include "MMA3/Widgets/Structures.h"
#include "MapButton.h"
#include "CustomTextBlock.h"
#include "MapList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMapButtonPressed);

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
		UCustomTextBlock* CSubName;

	UPROPERTY(meta = (BindWidget))
		UCustomTextBlock* CAuthor;

	UPROPERTY(meta = (BindWidget))
		UCustomTextBlock* CMapper;

	UPROPERTY(meta = (BindWidget))
		UCustomTextBlock* CBpm;

	UPROPERTY(meta = (BindWidget))
		UCustomTextBlock* CDuration;

	UPROPERTY()
		UMapList* m_ListReference;

	void SetUnselected();

	void SetSelected();

private:

	UPROPERTY()
		FMapInfo m_Info;

public:

	UFUNCTION()
		void SetData(FMapInfo p_MapInfo);

	UFUNCTION()
		FMapInfo GetData();
};

/**
 *
 */
UCLASS()
class MMA3_API UMapList : public UCustomScrollBox
{
	GENERATED_BODY()

public:

	UPROPERTY()
		UMapCell* SelectedCell = nullptr;

	UPROPERTY()
		FOnMapButtonPressed OnMapButtonPressedEvent;

};
