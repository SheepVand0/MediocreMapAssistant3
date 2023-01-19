// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Spacer.h"
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

	/////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class USpacer* NoMapSelectedSpacer;

	/////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomScrollBox* LevelsScrollBox;

	/////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomButton* EditButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomButton* DeleteButton;

	/////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* CoverPreview;

	/////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomTextBlock* SongName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomTextBlock* SongSubName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomTextBlock* SongAuthor;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomTextBlock* Mapper;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomTextBlock* SongDuration;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomTextBlock* SongBpm;

};
