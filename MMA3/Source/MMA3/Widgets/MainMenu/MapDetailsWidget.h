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
#include "MapDetailsWidget.generated.h"

/**
 *
 */
UCLASS()
class MMA3_API UMapDetailsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	FMapInfo m_Info;

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

};
