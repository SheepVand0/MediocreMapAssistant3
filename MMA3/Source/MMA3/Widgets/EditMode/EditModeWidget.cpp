// Fill out your copyright notice in the Description page of Project Settings.


#include "EditModeWidget.h"
#include "MMA3/Core/Controller/CController.h"
#include "MMA3/Core/MapperPawn/CMapperPawn.h"
#include "Blueprint/WidgetTree.h"


void UEditModeWidget::NativeConstruct()
{
	TimeSlider->OnValueChanged.AddDynamic(this, &UEditModeWidget::TimeSliderChanged);
	TimeSlider->OnMouseCaptureBegin.AddDynamic(this, &UEditModeWidget::OnTimeSliderGrab);
	TimeSlider->OnMouseCaptureEnd.AddDynamic(this, &UEditModeWidget::OnTimeSliderRelease);

	MappingGridScaleSlider->SetValue(GetConfig()->MappingGridScale);
	SoundVolumeSlider->SetValue(GetConfig()->SoundVolume);
	HitSoundVolumeSlider->SetValue(GetConfig()->HitSoundVolume);
	SoundVisDetailSlider->SetValue(GetConfig()->SoundVisDetail);
	SoundVisVerticesXSlider->SetValue(GetConfig()->SoundVisVerticesX);
	SoundVisVerticesYSlider->SetValue(GetConfig()->SoundVisVerticesY);
	SoundVisAttenuationSlider->SetValue(GetConfig()->SoundVisAttenuation);

	MappingGridScaleSlider->OnValueChanged.AddDynamic(this, &UEditModeWidget::EventNeedGridChange);
	SoundVolumeSlider->OnValueChanged.AddDynamic(this, &UEditModeWidget::EventSettingSliderChanged);
	HitSoundVolumeSlider->OnValueChanged.AddDynamic(this, &UEditModeWidget::EventSettingSliderChanged);
	SoundVisDetailSlider->OnValueChanged.AddDynamic(this, &UEditModeWidget::EventNeedSoundVisUpdate);
	SoundVisVerticesXSlider->OnValueChanged.AddDynamic(this, &UEditModeWidget::EventNeedSoundVisGeneration);
	SoundVisVerticesYSlider->OnValueChanged.AddDynamic(this, &UEditModeWidget::EventNeedSoundVisGeneration);
	SoundVisAttenuationSlider->OnValueChanged.AddDynamic(this, &UEditModeWidget::EventNeedSoundVisUpdate);

	SettingsPanel->SetVisibility(ESlateVisibility::Collapsed);

	LastModSliderValue = 0;

	TArray<float> l_Divisions = GetConfig()->MappingDivisions;
	for (int l_i = 0; l_i < l_Divisions.Num(); l_i++) {
		UCustomSpinBox* l_SpinBox = WidgetTree->ConstructWidget<UCustomSpinBox>();
		MappingDivisionsContainer->AddChildToVerticalBox(l_SpinBox);
		//l_SpinBox->SetSliderMaxValue(128);
		l_SpinBox->SetMaxValue(128);
		//l_SpinBox->SetSliderMinValue(1);
		l_SpinBox->SetMaxSliderValue(128);
		l_SpinBox->SetMaxSliderValue(128);
		l_SpinBox->SetMinValue(1);
		l_SpinBox->SetMinSliderValue(1);
		l_SpinBox->SetValue(l_Divisions[l_i]);
		MappingDivisionsSpinBoxes.Add(l_SpinBox);
	}

	UpdateSelectedMappingDivision();
}

void UEditModeWidget::OnTimeSliderGrab()
{
	TimeSliderGrabed = true;
}

void UEditModeWidget::OnTimeSliderRelease()
{
	TimeSliderGrabed = false;
	TimeSliderChanged(TimeSlider->GetValue());
}

void UEditModeWidget::TimeSliderChanged(float value)
{
	if (TimeSliderGrabed) {
		int l_ValueModTwo = (int)value % 2;

		if (l_ValueModTwo != LastModSliderValue) {
			OnTimeChanged.Broadcast(value);

			LastModSliderValue = l_ValueModTwo;
		}
	} else if (!IsControllerPlaying) {
		OnTimeChanged.Broadcast(value);
	}
}

void UEditModeWidget::EventSettingSliderChanged(float value)
{
	GetConfig()->SoundVolume = SoundVolumeSlider->GetValue();
	GetConfig()->HitSoundVolume = HitSoundVolumeSlider->GetValue();

	GetMappingController()->AudioComponent->Stop();
	GetMappingController()->AudioComponent->VolumeMultiplier = GetConfig()->SoundVolume;
	GetMappingController()->AudioComponent->Play(GetMappingController()->GetPlayTime());

	UMMAConfig::SaveConfig(GetConfig());
}

void UEditModeWidget::EventNeedGridChange(float value)
{
	//UE_LOG(LogTemp, Display, TEXT("Need grid change"));

	GetConfig()->MappingGridScale = MappingGridScaleSlider->GetValue();

	GetMappingController()->GenerateBeatGrid();
}

void UEditModeWidget::EventNeedSoundVisGeneration(float value)
{
	GetConfig()->SoundVisVerticesX = SoundVisVerticesXSlider->GetValue();
	GetConfig()->SoundVisVerticesY = SoundVisVerticesYSlider->GetValue();
	

	GetMappingController()->GenerateVisMesh();

	UMMAConfig::SaveConfig(GetConfig());
}

void UEditModeWidget::EventNeedSoundVisUpdate(float value)
{
	GetConfig()->SoundVisAttenuation = SoundVisAttenuationSlider->GetValue();
	GetConfig()->SoundVisDetail = SoundVisDetailSlider->GetValue();

	GetMappingController()->UpdateVisMesh();

	UMMAConfig::SaveConfig(GetConfig());
}

void UEditModeWidget::EventMappingDivisionSpinBoxChange(float value)
{
	int l_SelectedIndex = GetMapperPawn()->SelectedMappingDivisionIndex;

	GetConfig()->MappingDivisions[l_SelectedIndex] = value;
	UMMAConfig::SaveConfig(GetConfig());
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
	TimeText->SetText(FText::FromString(ACController::TimeToString(newTime)));
}

void UEditModeWidget::SwitchSettingsWidget()
{
	SettingsPanel->SetVisibility(IsInSettings ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	MainPanel->SetVisibility(IsInSettings ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	IsInSettings = !IsInSettings;
}

void UEditModeWidget::UpdateSelectedMappingDivision()
{
	for (int l_i = 0; l_i < MappingDivisionsSpinBoxes.Num(); l_i++) {
		UCustomSpinBox* l_SpinBox = MappingDivisionsSpinBoxes[l_i];

		auto l_WidgetStyle = l_SpinBox->GetWidgetStyle();

		if (l_i == (GetMapperPawn() ? GetMapperPawn()->SelectedMappingDivisionIndex : 0)) {
			l_WidgetStyle.InactiveFillBrush.TintColor = FSlateColor(FLinearColor(0, 0.5f, 0.8f));
		}
		else {
			l_WidgetStyle.InactiveFillBrush.TintColor = FSlateColor(FLinearColor(0.0f, 0, 0.0f));
		}

		l_SpinBox->OnValueChanged.Clear();
		l_SpinBox->SetValue(GetConfig()->MappingDivisions[l_i]);
		l_SpinBox->OnValueChanged.AddDynamic(this, &UEditModeWidget::EventMappingDivisionSpinBoxChange);
		
		l_SpinBox->SetWidgetStyle(l_WidgetStyle);
	}
}
