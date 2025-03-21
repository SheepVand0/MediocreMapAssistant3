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
#include "MMA3/Widgets/Components/CustomTextBlock.h"
#include "MMA3/Widgets/Components/MapButton.h"
#include "MMA3/Widgets/Structures.h"
#include "Components/Image.h"
#include "ImageUtils.h"
#include "RuntimeAudioImporter\Public\RuntimeAudioImporterLibrary.h"
#include "RuntimeAudioImporter\Public\RuntimeAudioImporterTypes.h"
#include "RuntimeAudioImporter\Public\Sound\ImportedSoundWave.h"
#include "MMA3/EditorLibrary.h"
#include "MMA3/Misc/SActionPtr.h"
#include "MMA3/EditorLibrary.h"
#include "MapDetailsWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNeedToAddDifficultyBeatmaps);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultySelected, FString, SerializedDifficulty);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMapCellsFinishedToBeAdd);

UCLASS()
class MMA3_API UMapDifficultyCell : public UUserWidget {

	GENERATED_BODY()

private:

	virtual void NativeConstruct() override;

	UPROPERTY()
		UMapDetailsWidget* ParentReference;

	UPROPERTY(meta  = (BindWidget))
		class UCustomButton* Difficulty;

	UPROPERTY(meta = (BindWidget))
		class UCustomTextBlock* DifficultyText;

	//////////////////////////////////////////////////

	UPROPERTY()
		FString SerializedDifficulty;

	UFUNCTION()
		void OnClicked();

public:

	UFUNCTION(BlueprintCallable)
		void SetDifficulty(UMapDetailsWidget* p_ParentReference, FString p_Difficulty);

	UFUNCTION(BlueprintPure)
		FString GetDifficulty();

};

/**
 *
 */
UCLASS()
class MMA3_API UMapDetailsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	static UMapDetailsWidget* Instance;
	
	UPROPERTY()
		FMapInfo Info;

	UPROPERTY()
		FString SelectedMode;

	UPROPERTY()
		FMapDifficulty m_SelectedDifficulty;

////////////////////////////////////////////////////////////////

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
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

	UPROPERTY(BlueprintAssignable)
		FOnDifficultySelected EventOnDifficultySelected;

	UPROPERTY(BlueprintAssignable)
		FOnMapCellsFinishedToBeAdd OnMapCellsFinished;

	UFUNCTION(BlueprintCallable)
		void OnMapCellsFnisihedToBeAddCallback();

	UFUNCTION()
		void SetMap(FMapInfo p_Map);

	UFUNCTION()
		void UpdateCover();

	UFUNCTION()
		void UpdateDifficultyList(FString p_Mode);

	UFUNCTION()
		void SetDifficultyPropertiesEnable(bool p_Enable);

	UFUNCTION()
		void OnDifficultySelected(FString SerializedDifficulty);

	UFUNCTION()
		void OnEditButtonClicked();

	UFUNCTION()
		void FinishedLoadingAudio(URuntimeAudioImporterLibrary* Importer, UImportedSoundWave* ImportedSoundWave, ERuntimeImportStatus Status);

};

UMapDetailsWidget* UMapDetailsWidget::Instance = nullptr;