// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSettings.h"

void UMainSettings::NativeConstruct() {
	GamePath->OnTextChanged.AddDynamic(this, &UMainSettings::OnGamePathChanged);

	OnNeedBlueprintInitialization();

	LoadConfig();
}

void UMainSettings::LoadConfig() {
	UMMAConfig* l_Config = UMMAConfig::LoadConfig();

	GamePath->SetText(FText::FromString(l_Config->GamePath));
}

void UMainSettings::SaveConfig() {
	UMMAConfig* l_Config = UMMAConfig::LoadConfig();
	l_Config->GamePath = GamePath->GetText().BuildSourceString();
	UMMAConfig::SaveConfig(l_Config);
}

void UMainSettings::OnGamePathChanged(const FText& x) {
	SaveConfig();
}