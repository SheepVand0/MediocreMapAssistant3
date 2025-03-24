// Fill out your copyright notice in the Description page of Project Settings.


#include "EditModeWidget.h"

void UEditModeWidget::NativeConstruct()
{
	TimeSlider->OnValueChanged.AddDynamic(this, &UEditModeWidget::TimeSliderChanged);
}

void UEditModeWidget::TimeSliderChanged(float value)
{
	if (!IsControllerPlaying)
		OnTimeChanged.Broadcast(value);
}

void UEditModeWidget::Init(float songDuration)
{
	TimeSlider->SetMaxValue(songDuration);
	TimeSlider->SetValue(0);
}

void UEditModeWidget::UpdateWidget(float newTime, bool isPlaying)
{
	IsControllerPlaying = isPlaying;
	TimeSlider->SetValue(newTime);
}
