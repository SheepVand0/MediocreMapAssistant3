// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMA3/Widgets/Components/CustomButton.h"
#include "MMA3/Widgets/Components/CustomSlider.h"
#include "MMA3/Widgets/Components/CustomSpinBox.h"
#include "MMA3/Widgets/Components/CustomComboString.h"
#include "MMA3/Widgets/Components/CustomScrollBox.h"
#include "MMA3/Widgets/Components/CustomEditableTextBox.h"
#include "MMA3/Widgets/Components/CustomTextBlock.h"
#include "MMA3/Widgets/Components/MapButton.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/UserWidget.h"
#include "MMA3/MMAConfig.h"
#include "EditModeWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChanged, float, newTime);

/**
 * 
 */
UCLASS()
class MMA3_API UEditModeWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	UFUNCTION()
	void OnTimeSliderGrab();

	UFUNCTION()
	void OnTimeSliderRelease();

	int LastModSliderValue;

protected:

	void NativeConstruct() override;

	bool IsInSettings;

////////////////////////////////////////////////////////////////////////////////////
/// MainWidget

	UPROPERTY(meta = (BindWidget))
	class UCustomSlider* TimeSlider;

	UPROPERTY(meta = (BindWidget))
	class UCustomTextBlock* TimeText;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* SettingsPanel;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* MainPanel;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* MappingDivisionsContainer;

	UPROPERTY()
	TArray<UCustomSpinBox*> MappingDivisionsSpinBoxes;

	UPROPERTY()
	bool IsControllerPlaying;

	UPROPERTY()
	bool TimeSliderGrabed;

	UFUNCTION()
	void TimeSliderChanged(float value);
////////////////////////////////////////////////////////////////////////////////////
/// Settings

	UPROPERTY(meta = (BindWidget))
	class UCustomSpinBox* SoundVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	class UCustomSpinBox* HitSoundVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	class UCustomSpinBox* MappingGridScaleSlider;

	UPROPERTY(meta = (BindWidget))
	class UCustomSpinBox* SoundVisDetailSlider;

	UPROPERTY(meta = (BindWidget))
	class UCustomSpinBox* SoundVisVerticesXSlider;

	UPROPERTY(meta = (BindWidget))
	class UCustomSpinBox* SoundVisVerticesYSlider;

	UPROPERTY(meta = (BindWidget))
	class UCustomSpinBox* SoundVisAttenuationSlider;

	UFUNCTION()
	void EventSettingSliderChanged(float value);

	UFUNCTION()
	void EventNeedGridChange(float value);

	UFUNCTION()
	void EventNeedSoundVisGeneration(float value);

	UFUNCTION()
	void EventNeedSoundVisUpdate(float value);

	UFUNCTION()
	void EventMappingDivisionSpinBoxChange(float value);

public:

	UPROPERTY(BlueprintAssignable)
	FOnTimeChanged OnTimeChanged;

	UFUNCTION()
	void Init(float songDuration);

	UFUNCTION()
	void UpdateWidget(float newTime, bool isPlaying);

	UFUNCTION()
	void SwitchSettingsWidget();

	UFUNCTION()
	void UpdateSelectedMappingDivision();

};
