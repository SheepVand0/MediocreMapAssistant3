// Fill out your copyright notice in the Description page of Project Settings.


#include "MapDetailsWidget.h"

void UMapDifficultyCell::SetData(UMapDetailsWidget* p_ParentReference, FString p_Difficulty) {
	ParentReference = p_ParentReference;
	SerializedDifficulty = p_Difficulty;
}

FString UMapDifficultyCell::GetDifficulty() {
	return SerializedDifficulty;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void UMapDetailsWidget::NativeConstruct() {
	EventOnDifficultySelected.AddDynamic(this, &UMapDetailsWidget::OnDifficultySelected);
}

void UMapDetailsWidget::SetMap(FMapInfo p_Map) {

	m_Info = p_Map;

	MapName->SetText(FText::FromString(p_Map.SongName));
	MapSubName->SetText(FText::FromString(p_Map.SongSubName));
	AuthorName->SetText(FText::FromString(p_Map.SongAuthor));
	Mapper->SetText(FText::FromString(p_Map.SongMapper));
	SongPath->SetText(FText::FromString(p_Map.AudioFileName));
	CoverFile->SetText(FText::FromString(p_Map.CoverImageFileName));

	UpdateMapList("Standard");

	UpdateCover();
}

void UMapDetailsWidget::SetDifficultyPropertiesEnable(bool p_Enable) {
	BeatmapVersion->SetIsEnabled(p_Enable);
	StandardModeButton->SetIsEnabled(p_Enable);
	LawlessModeButton->SetIsEnabled(p_Enable);
	ThreeSixteenModeButton->SetIsEnabled(p_Enable);
	NinetyModeButton->SetIsEnabled(p_Enable);
	LightshowModeButton->SetIsEnabled(p_Enable);
	CMapDifficulty->SetIsEnabled(p_Enable);
	CNJSSlider->SetIsEnabled(p_Enable);
	COffsetSlider->SetIsEnabled(p_Enable);
	CDifficultyList->SetIsEnabled(p_Enable);
	CEditButton->SetIsEnabled(p_Enable);
	BeatmapVersion->SetIsEnabled(p_Enable);
}

void UMapDetailsWidget::UpdateMapList(FString p_Mode) {

	FDifficultyBeatmapSet l_SelectedSet;
	for (int l_i = 0; l_i < m_Info.DifficultyBeatmapSets.Num(); l_i++) {
		if (m_Info.DifficultyBeatmapSets[l_i].Name != p_Mode) continue;

		l_SelectedSet = m_Info.DifficultyBeatmapSets[l_i];
	}

	ListBeatmapDifficultiesSerialized.Empty();
	for (int l_i = 0; l_i < l_SelectedSet.DifficultyBeatmaps.Num(); l_i++) {
		ListBeatmapDifficultiesSerialized.Add(l_SelectedSet.DifficultyBeatmaps[l_i].Difficulty);
	}

	CDifficultyList->ClearChildren();

	bool l_NeedAdd = (ListBeatmapDifficultiesSerialized.Num() == 0) == false;
	SetDifficultyPropertiesEnable(l_NeedAdd);

	if (l_NeedAdd) {
		OnNeedToAddDifficultyBeatmaps.Broadcast();

		for (int l_i = 0; l_i < ListBeatmapDifficultiesSerialized.Num(); l_i++) {
			UMapDifficultyCell* l_Cell = Cast<UMapDifficultyCell>(CDifficultyList->GetChildAt(l_i));
			if (l_Cell == nullptr) continue;
			l_Cell->SetData(this, ListBeatmapDifficultiesSerialized[l_i]);
		}
	}
}

void UMapDetailsWidget::UpdateCover() {

	FString l_Path = m_Info.MapPath + "\\" + m_Info.CoverImageFileName;
	m_Info.Cover = FImageUtils::ImportFileAsTexture2D(*l_Path);

	if (m_Info.Cover != nullptr) {

		CoverPreview->SetBrushFromTexture(m_Info.Cover);
		CoverNotFound->SetVisibility(ESlateVisibility::Collapsed);
	}
	else {
		CoverNotFound->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMapDetailsWidget::OnDifficultySelected(FString SerializedDifficulty) {
	SetDifficultyPropertiesEnable(true);


}