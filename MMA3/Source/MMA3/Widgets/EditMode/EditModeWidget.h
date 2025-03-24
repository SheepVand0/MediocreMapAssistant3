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
#include "Blueprint/UserWidget.h"
#include "EditModeWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChanged, float, newTime);

/**
 * 
 */
UCLASS()
class MMA3_API UEditModeWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:

	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UCustomSlider* TimeSlider;

	UPROPERTY()
	bool IsControllerPlaying;

	UFUNCTION()
	void TimeSliderChanged(float value);

public:

	UPROPERTY(BlueprintAssignable)
	FOnTimeChanged OnTimeChanged;

	UFUNCTION()
	void Init(float songDuration);

	UFUNCTION()
	void UpdateWidget(float newTime, bool isPlaying);

};
