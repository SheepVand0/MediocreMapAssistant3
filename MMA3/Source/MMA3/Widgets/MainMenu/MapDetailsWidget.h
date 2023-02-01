// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MMA3/Widgets/Components/CustomButton.h"
#include "MMA3/Widgets/Components/CustomSlider.h"
#include "MMA3/Widgets/Components/CustomSpinBox.h"
#include "MMA3/Widgets/Components/CustomComboString.h"
#include "MMA3/Widgets/Components/CustomScrollBox.h"
#include "MMA3/Widgets/Components/CustomEditableTextBox.h"
#include "MMA3/Widgets/Structures.h"
#include "Components/Image.h"
#include "ImageUtils.h"
#include "MapDetailsWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNeedToAddDifficultyBeatmaps)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultySelected, FString, SerializedDifficulty)

/**
 *
 */
UCLASS()
class MMA3_API UMapDetailsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FMapInfo m_Info;

	UPROPERTY()
		FMapDifficulty m_SelectedDifficulty;

	UPROPERTY(EditAnywhere, meta =(BindWidget))
		class UCustomEditableTextBox* MapName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomEditableTextBox* MapSubName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomEditableTextBox* AuthorName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomEditableTextBox* Mapper;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomSpinBox* BPM;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomSpinBox* PreviewTime;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomSpinBox* PreviewDuration;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomEditableTextBox* EnvironmentName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomButton* ShowEnvironmentList;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomEditableTextBox* SongPath;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomEditableTextBox* CoverFile;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* CoverPreview;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomTextBlock* AudioNotFound;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomTextBlock* CoverNotFound;

	//////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomComboString* BeatmapVersion;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomComboString* CMapDifficulty;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomButton* CEditButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomSpinBox* CNJSSlider;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomSpinBox* COffsetSlider;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomScrollBox* CDifficultyList;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomEditableTextBox* CDifficultyLabel;

	//////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomButton* StandardModeButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomButton* LawlessModeButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomButton* LightshowModeButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomButton* ThreeSixteenModeButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomButton* NinetyModeButton;

	//////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly)
		TArray<FString> ListBeatmapDifficultiesSerialized;

	UPROPERTY(BlueprintAssignable)
		FOnNeedToAddDifficultyBeatmaps OnNeedToAddDifficultyBeatmaps;

	UFUNCTION()
		void SetMap(FMapInfo p_Map);

	UFUNCTION()
		void UpdateCover();

	UFUNCTION()
		void UpdateMapList(FString p_Mode);

	UFUNCTION()
		void SetDifficultyPropertiesEnable(bool p_Enable);

};
