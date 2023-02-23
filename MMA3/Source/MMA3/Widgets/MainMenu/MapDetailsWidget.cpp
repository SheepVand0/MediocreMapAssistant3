// Fill out your copyright notice in the Description page of Project Settings.


#include "MapDetailsWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MMA3/Core/C_Controller.h"


void UMapDifficultyCell::NativeConstruct() {
	Difficulty->OnClicked.AddDynamic(this, &UMapDifficultyCell::OnClicked);
}

void UMapDifficultyCell::SetMapInfo(UMapDetailsWidget* p_ParentReference, FString p_Difficulty) {
	ParentReference = p_ParentReference;
	SerializedDifficulty = p_Difficulty;
	DifficultyText->SetText(FText::FromString(p_Difficulty));
}

FString UMapDifficultyCell::GetDifficulty() {
	return SerializedDifficulty;
}

void UMapDifficultyCell::OnClicked() {
	ParentReference->EventOnDifficultySelected.Broadcast(SerializedDifficulty);
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void UMapDetailsWidget::NativeConstruct() {
	EventOnDifficultySelected.AddDynamic(this, &UMapDetailsWidget::OnDifficultySelected);
	CEditButton->OnClicked.AddDynamic(this, &UMapDetailsWidget::OnEditButtonClicked);
	RuntimeAudioImporter = URuntimeAudioImporterLibrary::CreateRuntimeAudioImporter();

	Instance = this;

	SetVisibility(ESlateVisibility::Collapsed);
}

void UMapDetailsWidget::SetMap(FMapInfo p_Map) {

	m_Info = p_Map;

	MapName->SetText(FText::FromString(p_Map.SongName));
	MapSubName->SetText(FText::FromString(p_Map.SongSubName));
	AuthorName->SetText(FText::FromString(p_Map.SongAuthor));
	Mapper->SetText(FText::FromString(p_Map.SongMapper));
	SongPath->SetText(FText::FromString(p_Map.AudioFileName));
	CoverFile->SetText(FText::FromString(p_Map.CoverImageFileName));
	BPM->SetValue(p_Map.BPM);
	PreviewDuration->SetValue(p_Map.PreviewDuration);
	PreviewTime->SetValue(p_Map.PreviewStartTime);

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
	CNJSSlider->bEnableSlider = p_Enable;
	COffsetSlider->bEnableSlider = p_Enable;
	CDifficultyList->SetIsEnabled(p_Enable);
	CEditButton->SetIsEnabled(p_Enable);
}

void UMapDetailsWidget::UpdateMapList(FString p_Mode) {

	m_SelectedMode = p_Mode;
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

	bool l_IsZero = ListBeatmapDifficultiesSerialized.Num() == 0;

	if (l_IsZero)
		SetDifficultyPropertiesEnable(false);

	OnNeedToAddDifficultyBeatmaps.Broadcast();

}

void UMapDetailsWidget::OnMapCellsFnisihedToBeAddCallback() {
	if (ListBeatmapDifficultiesSerialized.Num() > 0)
		OnDifficultySelected(ListBeatmapDifficultiesSerialized[0]);
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
	//GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::White, FString(SerializedDifficulty));
	SetDifficultyPropertiesEnable(true);
	FMapDifficulty l_Difficulty = GetDifficultyByMapAndMode(SerializedDifficulty, m_SelectedMode);
	m_SelectedDifficulty = l_Difficulty;

	/*GEngine->AddOnScreenDebugMessage(2, 10.0f, FColor::White, FString(FString::SanitizeFloat(l_Difficulty.NoteJumpMovementSpeed)));
	GEngine->AddOnScreenDebugMessage(3, 10.0f, FColor::White, FString(FString::SanitizeFloat(l_Difficulty.NoteJumpStartBeatOffset)));*/
	bool l_Found = false;
	for (int l_i = 0; l_i < CMapDifficulty->GetOptionCount(); l_i++) {
		if (CMapDifficulty->GetOptionAtIndex(l_i) == SerializedDifficulty) {
			CMapDifficulty->SetSelectedIndex(l_i);
			l_Found = true;
			break;
		}
	}
	if (!l_Found)
		CMapDifficulty->SetSelectedOption(SerializedDifficulty);
	CNJSSlider->SetValue(l_Difficulty.NoteJumpMovementSpeed);
	COffsetSlider->SetValue(l_Difficulty.NoteJumpStartBeatOffset);
	CDifficultyLabel->SetText(FText::FromString(l_Difficulty.DifficultyLabel));
}

FMapDifficulty UMapDetailsWidget::GetDifficultyByMapAndMode(FString p_Difficulty, FString p_Mode) {
	FMapDifficulty l_Difficulty;
	bool l_Finished = false;
	//GEngine->AddOnScreenDebugMessage(4, 10.0f, FColor::Green, FString::SanitizeFloat(ListBeatmapDifficultiesSerialized.Num()));;
	for (int l_i = 0; l_i < m_Info.DifficultyBeatmapSets.Num(); l_i++) {
		//GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::White, FString(p_Mode + " / " + m_Info.DifficultyBeatmapSets[l_i].Name));
		if (m_Info.DifficultyBeatmapSets[l_i].Name != p_Mode) continue;

		TArray<FMapDifficulty> l_DifficultyList = m_Info.DifficultyBeatmapSets[l_i].DifficultyBeatmaps;

		for (int l_i1 = 0; l_i1 < l_DifficultyList.Num(); l_i1++) {
			//GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::White, FString(p_Difficulty + " / " + l_DifficultyList[l_i1].Difficulty));
			if (l_DifficultyList[l_i1].Difficulty != p_Difficulty) continue;

			l_Finished = true;

			l_Difficulty = m_Info.DifficultyBeatmapSets[l_i].DifficultyBeatmaps[l_i1];
		}
	}

	//GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::White, FString(l_Finished ? "True" : "False"));
	if (l_Finished == false) return FMapDifficulty{};

	return l_Difficulty;
}

void UMapDetailsWidget::OnEditButtonClicked() {
	//GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor::White, FString(m_Info.MapPath + "\\" + m_Info.AudioFileName));
	if (m_Info.Song == nullptr) {
		RuntimeAudioImporter->OnResult.AddDynamic(this, &UMapDetailsWidget::FinishedLoadingAudio);
		RuntimeAudioImporter->ImportAudioFromFile(m_Info.MapPath + "\\" + m_Info.AudioFileName, EAudioFormat::OggVorbis);
	}
}

void UMapDetailsWidget::FinishedLoadingAudio(URuntimeAudioImporterLibrary* p_Importer, UImportedSoundWave* p_ImportedSoundWave, ETranscodingStatus p_Status) {
	if (p_ImportedSoundWave == nullptr) { UE_LOG(LogTemp, Error, TEXT("Sound File not found")); return; }
	//PlaySound(p_ImportedSoundWave);
	m_Info.Song = p_ImportedSoundWave;
	RuntimeAudioImporter->OnResult.RemoveAll(this);
	AC_Controller* l_Controller = Cast<AC_Controller>(UGameplayStatics::GetActorOfClass(GetWorld(), AC_Controller::StaticClass()));
	l_Controller->GenerateGrid(m_Info, m_SelectedDifficulty.Difficulty, m_SelectedMode);
	SetVisibility(ESlateVisibility::Collapsed);
	l_Controller->CurrentScene = "Editing";
}