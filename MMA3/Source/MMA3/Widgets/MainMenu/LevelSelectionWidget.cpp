// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSelectionWidget.h"
#include "MMA3/MMAConfig.h"
#include "Kismet/GameplayStatics.h"

void ULevelSelectionWidget::PostLoad() {
	Super::PostLoad();

	//return;

	if (UMMAConfig::Instance == nullptr) {
		bool l_SaveGameExists = UGameplayStatics::DoesSaveGameExist(MMA_SAVE_GAME_SLOT_NAME, 0);

		if (l_SaveGameExists) { UMMAConfig::Instance = (UMMAConfig*)UGameplayStatics::LoadGameFromSlot(MMA_SAVE_GAME_SLOT_NAME, 0); }
		else {
			UMMAConfig::Instance = (UMMAConfig*)UGameplayStatics::CreateSaveGameObject(UMMAConfig::StaticClass());
			UGameplayStatics::SaveGameToSlot(UMMAConfig::Instance, MMA_SAVE_GAME_SLOT_NAME, 0);
		}
	}

	ULevelSelectionWidget::Instance = this;

	MapDetailsBox->SetVisibility(ESlateVisibility::Collapsed);

	LevelsScrollBox->OnMapButtonPressedEvent.AddDynamic(this, &ULevelSelectionWidget::OnMapSelected);

	RefreshMaps(EMapListType::WIP);
}

void ULevelSelectionWidget::OnMapSelected() {
	MapDetailsBox->SetVisibility(ESlateVisibility::Visible);
	FMapInfo l_Info = LevelsScrollBox->SelectedCell->GetData();
	SongName->SetText(FText::FromString(l_Info.SongName));
	SongSubName->SetText(FText::FromString(l_Info.SongSubName));
	SongAuthor->SetText(FText::FromString(l_Info.SongAuthor));
	Mapper->SetText(FText::FromString(l_Info.SongMapper));
	SongDuration->SetText(FText::FromString("0:00"));
	SongBpm->SetText(FText::FromString(FString::SanitizeFloat(l_Info.BPM)));
}

void ULevelSelectionWidget::RefreshMaps(EMapListType p_MapRefreshType) {

	m_Maps.Empty();

	FString l_Path = UMMAConfig::Instance->GamePath + "\\Beat Saber_Data" + (p_MapRefreshType == EMapListType::WIP ? "\\CustomWIPLevels" : "\\CustomLevels");

	GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor::White, l_Path);

	FDirectoryVisitor l_Visitor;

	IPlatformFile& l_FileManager = FPlatformFileManager::Get().GetPlatformFile();

	l_FileManager.IterateDirectory(*l_Path, l_Visitor);

	LevelsScrollBox->ClearChildren();

	for (int l_i = 0; l_i < m_Maps.Num(); l_i++) {

		OnMapCellNeedToBeAdded.Broadcast();
	}

};