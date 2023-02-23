// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSelectionWidget.h"
#include "MMA3/MMAConfig.h"
#include "Engine/Texture2D.h"
#include "ImageUtils.h"
#include "MMA3/Widgets/MainMenu/MapDetailsWidget.h"
#include "MMA3/Widgets/WidgetUtils.h"
#include "Kismet/GameplayStatics.h"

void ULevelSelectionWidget::NativeConstruct() {
	//Super::PostLoad();

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

	OnBlueprintReady.AddDynamic(this, &ULevelSelectionWidget::EventBlueprintReady);

	EditButton->OnClicked.AddDynamic(this, &ULevelSelectionWidget::OnEditButtonPressed);
}

void ULevelSelectionWidget::EventBlueprintReady() {
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

	if (l_Info.Cover == nullptr) {
		IPlatformFile& l_FileManager = FPlatformFileManager::Get().GetPlatformFile();

		FString l_FilePath = (l_Info.MapPath + "\\" + l_Info.CoverImageFileName);

		//GEngine->AddOnScreenDebugMessage(2, 10.0f, FColor::White, FString(l_FilePath));

		//if (!l_FileManager.FileExists(*l_FilePath)) return;

		l_Info.Cover = FImageUtils::ImportFileAsTexture2D(*l_FilePath);
	}

	if (l_Info.Cover == nullptr) return;

	//GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::White, FString(l_Info.Cover == nullptr ? "True" : "False"));
	//GEngine->AddOnScreenDebugMessage(2, 10.0f, FColor::White, FString(CoverPreview == nullptr ? "True" : "False"));

	FSlateBrush l_Brush = UWidgetBlueprintLibrary::MakeBrushFromTexture(l_Info.Cover, l_Info.Cover->GetSizeX(), l_Info.Cover->GetSizeY());
	l_Brush = WidgetUtils::GetUIElementStyle(l_Brush, 255, FColor::White, FMargin(0));
	l_Brush.DrawAs = ESlateBrushDrawType::RoundedBox;
	l_Brush.OutlineSettings.CornerRadii = FVector4d(10, 10, 10, 10);
	l_Brush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;

	CoverPreview->SetBrush(l_Brush);
}

void ULevelSelectionWidget::RefreshMaps(EMapListType p_MapRefreshType) {

	m_Maps.Empty();

	FString l_Path = UMMAConfig::Instance->GamePath + "\\Beat Saber_Data" + (p_MapRefreshType == EMapListType::WIP ? "\\CustomWIPLevels" : "\\CustomLevels");

	//GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor::White, l_Path);

	FDirectoryVisitor l_Visitor;

	IPlatformFile& l_FileManager = FPlatformFileManager::Get().GetPlatformFile();

	l_FileManager.IterateDirectory(*l_Path, l_Visitor);

	LevelsScrollBox->ClearChildren();

	for (int l_i = 0; l_i < m_Maps.Num(); l_i++) {

		OnMapCellNeedToBeAdded.Broadcast();
	}

	for (int l_i = 0; l_i < LevelsScrollBox->GetChildrenCount(); l_i++) {

		UMapCell* l_Cell = Cast<UMapCell>(LevelsScrollBox->GetChildAt(l_i));
		if (l_Cell == nullptr) continue;

		l_Cell->m_ListReference = LevelsScrollBox;
		l_Cell->SetData(m_Maps[l_i]);
	}

};

void ULevelSelectionWidget::OnEditButtonPressed() {
	UMapDetailsWidget::Instance->SetVisibility(ESlateVisibility::Visible);
	this->SetVisibility(ESlateVisibility::Collapsed);

	UMapDetailsWidget::Instance->SetMap(LevelsScrollBox->SelectedCell->GetData());
};