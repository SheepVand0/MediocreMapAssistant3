// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSelectionWidget.h"
#include "MMA3/MMAConfig.h"
#include "Kismet/GameplayStatics.h"

void ULevelSelectionWidget::PostLoad() {
	if (UMMAConfig::Instance == nullptr) {
		bool l_SaveGameExists = UGameplayStatics::DoesSaveGameExist(MMA_SAVE_GAME_SLOT_NAME, 0);

		if (l_SaveGameExists) { UMMAConfig::Instance = (UMMAConfig*)UGameplayStatics::LoadGameFromSlot(MMA_SAVE_GAME_SLOT_NAME, 0); }
		else {
			UMMAConfig::Instance = (UMMAConfig*)UGameplayStatics::CreateSaveGameObject(UMMAConfig::StaticClass());
			UGameplayStatics::SaveGameToSlot(UMMAConfig::Instance, MMA_SAVE_GAME_SLOT_NAME, 0);
		}
	}

	s_FileManager = FPlatformFileManager::Get().GetPlatformFile();

	Instance = this;

	RefreshMaps(EMapListType::WIP);
}

void ULevelSelectionWidget::RefreshMaps(EMapListType p_MapRefreshType) {

	FString l_Path = UMMAConfig::Instance->GamePath + (p_MapRefreshType == EMapListType::WIP ? "CustomWIPLevels" : "CustomLevels");

	FDirectoryVisitor l_Visitor;

	s_FileManager.IterateDirectory(*l_Path, l_Visitor);

};