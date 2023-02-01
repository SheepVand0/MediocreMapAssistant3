// Fill out your copyright notice in the Description page of Project Settings.


#include "MapDetailsWidget.h"

void UMapDetailsWidget::SetMap(FMapInfo p_Map) {

	m_Info = p_Map;

	MapName->SetText(FText::FromString(p_Map.SongName));
	MapSubName->SetText(FText::FromString(p_Map.SongSubName));
	AuthorName->SetText(FText::FromString(p_Map.SongAuthor));
	Mapper->SetText(FText::FromString(p_Map.SongMapper));
	SongPath->SetText(FText::FromString(p_Map.AudioFileName));
	CoverFile->SetText(FText::FromString(p_Map.CoverImageFileName));

}

void UMapDetailsWidget::SetDifficultyPropertiesEnable(bool p_Disable) {

	BeatmapVersion->SetIsEnabled(p_Disable);
	StandardModeButton->SetIsEnabled(p_Disable);
	LawlessModeButton->SetIsEnabled(p_Disable);
	ThreeSixteenModeButton->SetIsEnabled(p_Disable);
	NinetyModeButton->SetIsEnabled(p_Disable);
	LightshowModeButton->SetIsEnabled(p_Disable);
	CMapDifficulty->SetIsEnabled(p_Disable);
	CNJSSlider->SetIsEnabled(p_Disable);
	COffsetSlider->SetIsEnabled(p_Disable);
	CDifficultyList->SetIsEnabled(p_Disable);
	CEditButton->SetIsEnabled(p_Disable);
	BeatmapVersion->SetIsEnabled(p_Disable);
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

	if (l_NeedAdd)
		OnNeedToAddDifficultyBeatmaps.Broadcast();
}

void UMapDetailsWidget::UpdateCover() {

	FString l_Path = m_Info.MapPath + "\\" + m_Info.CoverImageFileName;
	m_Info.Cover = FImageUtils::ImportFileAsTexture2D(*l_Path);

	if (m_Info.Cover != nullptr) {

		CoverPreview->SetBrushFromTexture(m_Info.Cover);

	}

}