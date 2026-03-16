// Fill out your copyright notice in the Description page of Project Settings.


#include "MapDetailsWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MMA3/Core/Controller/CController.h"
#include <AudioDecompress.h>

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/// Map Difficulty Cell

void UMapDifficultyCell::NativeConstruct() {
	Difficulty->OnClicked.AddDynamic(this, &UMapDifficultyCell::OnClicked);
}

void UMapDifficultyCell::SetDifficulty(UMapDetailsWidget* p_ParentReference, FString p_Difficulty) {
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

void UMapDetailsWidget::EventFloatSliderChanged(float ign)
{
	if (IsLoadingData) return;

	if (Info->DifficultyBeatmapSets.IsValidIndex(SelectedModeIndex))
		if (Info->DifficultyBeatmapSets[SelectedModeIndex]->DifficultyBeatmaps.IsValidIndex(SelectedDifficultyIndex)) {
			Info->DifficultyBeatmapSets[SelectedModeIndex]->DifficultyBeatmaps[SelectedDifficultyIndex]->_noteJumpMovementSpeed = CNJSSlider->GetValue();
			Info->DifficultyBeatmapSets[SelectedModeIndex]->DifficultyBeatmaps[SelectedDifficultyIndex]->_noteJumpStartBeatOffset = COffsetSlider->GetValue();
		}

	Info->_beatsPerMinute = BPM->GetValue();
	Info->_previewDuration = PreviewDuration->GetValue();
	Info->_previewStartTime = PreviewTime->GetValue();
}

void UMapDetailsWidget::EventTextBoxChanged(const FText& text)
{
	if (IsLoadingData) return;

	if (Info->DifficultyBeatmapSets.IsValidIndex(SelectedModeIndex))
		if (Info->DifficultyBeatmapSets[SelectedModeIndex]->DifficultyBeatmaps.IsValidIndex(SelectedDifficultyIndex))
			if (Info->DifficultyBeatmapSets[SelectedModeIndex]->DifficultyBeatmaps[SelectedDifficultyIndex]->DifficultyCustomData) {
				Info->DifficultyBeatmapSets[SelectedModeIndex]->DifficultyBeatmaps[SelectedDifficultyIndex]->DifficultyCustomData->DifficultyLabel = *CDifficultyLabel->GetText().ToString();
			}



	Info->_songFileName = *SongPath->GetText().ToString();
	Info->_coverImageFileName = *CoverFile->GetText().ToString();
	Info->_songName = *MapName->GetText().ToString();
	Info->_songSubName = *MapSubName->GetText().ToString();
	Info->_songAuthorName = *AuthorName->GetText().ToString();
	Info->_levelAuthorName = *Mapper->GetText().ToString();
	Info->_environmentName = *EnvironmentName->GetText().ToString();
}

void UMapDetailsWidget::NativeConstruct() {
	EventOnDifficultySelected.AddDynamic(this, &UMapDetailsWidget::OnDifficultySelected);
	CEditButton->OnClicked.AddDynamic(this, &UMapDetailsWidget::OnEditButtonClicked);
	OnMapCellsFinished.AddDynamic(this, &UMapDetailsWidget::OnMapCellsFnisihedToBeAddCallback);

	CNJSSlider->OnValueChanged.AddDynamic(this, &UMapDetailsWidget::EventFloatSliderChanged);
	COffsetSlider->OnValueChanged.AddDynamic(this, &UMapDetailsWidget::EventFloatSliderChanged);

	SongPath->OnTextChanged.AddDynamic(this, &UMapDetailsWidget::EventTextBoxChanged);
	CoverFile->OnTextChanged.AddDynamic(this, &UMapDetailsWidget::EventTextBoxChanged);
	MapName->OnTextChanged.AddDynamic(this, &UMapDetailsWidget::EventTextBoxChanged);
	MapSubName->OnTextChanged.AddDynamic(this, &UMapDetailsWidget::EventTextBoxChanged);
	AuthorName->OnTextChanged.AddDynamic(this, &UMapDetailsWidget::EventTextBoxChanged);
	Mapper->OnTextChanged.AddDynamic(this, &UMapDetailsWidget::EventTextBoxChanged);
	EnvironmentName->OnTextChanged.AddDynamic(this, &UMapDetailsWidget::EventTextBoxChanged);
	CDifficultyLabel->OnTextChanged.AddDynamic(this, &UMapDetailsWidget::EventTextBoxChanged);

	Instance = this;

	SetVisibility(ESlateVisibility::Collapsed);
}

void UMapDetailsWidget::SetMap(FMapInfo* p_Map) {
	IsLoadingData = true;
	Info = p_Map;

	MapName->SetText(FText::FromString(p_Map->_songName));
	MapSubName->SetText(FText::FromString(p_Map->_songSubName));
	AuthorName->SetText(FText::FromString(p_Map->_songAuthorName));
	Mapper->SetText(FText::FromString(p_Map->_levelAuthorName));
	SongPath->SetText(FText::FromString(p_Map->_songFileName));
	CoverFile->SetText(FText::FromString(p_Map->_coverImageFileName));
	BPM->SetValue(p_Map->_beatsPerMinute);
	PreviewDuration->SetValue(p_Map->_previewDuration);
	PreviewTime->SetValue(p_Map->_previewStartTime);

	UpdateDifficultyList("Standard");

	UpdateCover();
	IsLoadingData = false;
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

void UMapDetailsWidget::UpdateDifficultyList(FString p_Mode) {

	SelectedMode = p_Mode;
	FDifficultyBeatmapSet* l_SelectedSet = nullptr;
	for (int l_i = 0; l_i < Info->DifficultyBeatmapSets.Num(); l_i++) {
		if (Info->DifficultyBeatmapSets[l_i]->Name != p_Mode) continue;

		l_SelectedSet = Info->DifficultyBeatmapSets[l_i];
	}

	if (!l_SelectedSet) {
		return;
	}

	ListBeatmapDifficultiesSerialized.Empty();
	for (int l_i = 0; l_i < l_SelectedSet->DifficultyBeatmaps.Num(); l_i++) {
		ListBeatmapDifficultiesSerialized.Add(l_SelectedSet->DifficultyBeatmaps[l_i]->_difficulty);
	}

	CDifficultyList->ClearChildren();

	bool l_IsZero = ListBeatmapDifficultiesSerialized.Num() == 0;

	if (l_IsZero)
		SetDifficultyPropertiesEnable(false);

	OnNeedToAddDifficultyBeatmaps.Broadcast();

	if (l_SelectedSet->DifficultyBeatmaps.Num() != 0)
		OnDifficultySelected(l_SelectedSet->DifficultyBeatmaps[0]->_difficulty);

}

void UMapDetailsWidget::OnMapCellsFnisihedToBeAddCallback() {
	if (ListBeatmapDifficultiesSerialized.Num() > 0)
		OnDifficultySelected(ListBeatmapDifficultiesSerialized[0]);
}

void UMapDetailsWidget::UpdateCover() {

	FString l_Path = Info->MapPath + "\\" + Info->_coverImageFileName;
	Info->Cover = FImageUtils::ImportFileAsTexture2D(*l_Path);

	if (Info->Cover != nullptr) {
		CoverPreview->SetBrushFromTexture(Info->Cover);
		CoverNotFound->SetVisibility(ESlateVisibility::Collapsed);
	}
	else {
		CoverNotFound->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMapDetailsWidget::OnDifficultySelected(FString SerializedDifficulty) {
	IsLoadingData = true;
	SetDifficultyPropertiesEnable(true);
	FMapDifficulty* l_Difficulty = AEditorLibrary::GetDifficultyByMapAndMode(Info, SerializedDifficulty, SelectedMode);
	m_SelectedDifficulty = l_Difficulty;

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
	CNJSSlider->GetSpinBox()->SetValue(l_Difficulty->_noteJumpMovementSpeed);
	COffsetSlider->GetSpinBox()->SetValue(l_Difficulty->_noteJumpStartBeatOffset);
	UE_LOG(LogTemp, Display, TEXT("Start offset: %f"), l_Difficulty->_noteJumpStartBeatOffset);
	if (l_Difficulty->DifficultyCustomData) {
		CDifficultyLabel->SetText(FText::FromString(l_Difficulty->DifficultyCustomData->DifficultyLabel));
	}
	IsLoadingData = false;
}

void UMapDetailsWidget::OnEditButtonClicked() {
	AEditorLibrary::LoadAudio(Info->MapPath + "\\" + Info->_songFileName, ERuntimeAudioFormat::OggVorbis, (new SMethodPtr(this, &UMapDetailsWidget::FinishedLoadingAudio))->GetPointer());

	FFileHelper::SaveStringToFile(Info->GetJson(), *(Info->MapPath + "\\" + "Info->dat"), FFileHelper::EEncodingOptions::ForceUTF8);
}

void UMapDetailsWidget::FinishedLoadingAudio(URuntimeAudioImporterLibrary* Importer, UImportedSoundWave* ImportedSoundWave, ERuntimeImportStatus Status) {
	if (ImportedSoundWave == nullptr || Status != ERuntimeImportStatus::SuccessfulImport) {
		UE_LOG(LogTemp, Error, TEXT("Sound File not found"));
		GEngine->AddOnScreenDebugMessage(3, 1, FColor::Red, FString("Error Loading Audio"));
		return;
	}
	//ImportedSoundWave->OnGeneratePCMData.AddDynamic(GetMappingController(), &ACController::);

	Info->Song = ImportedSoundWave;
	int l_Samples = GEngine->GetMainAudioDevice().GetAudioDevice()->GetSampleRate();

	TArray<uint8> l_SongPCMData;
	Info->NeededSamples = ImportedSoundWave->OnGeneratePCMAudio(l_SongPCMData, l_Samples);
	Info->SongPCMData = l_SongPCMData.GetData();
	Info->PCMNumberOfValues = l_SongPCMData.Num();

	TArrayView<float> l_PCMDataView = ImportedSoundWave->GetPCMBuffer().PCMData.GetView();
	UE_LOG(LogTemp, Display, TEXT("PCM Data count: %d"), l_PCMDataView.Num());
	UE_LOG(LogTemp, Display, TEXT("PCM Data middle value: %f"), l_PCMDataView[l_PCMDataView.Num() / 2]);

	/// TODO : Move audio load in other class

	GEngine->AddOnScreenDebugMessage(0, 10, FColor::White, FString("Editing"));
	ACController* l_Controller = Cast<ACController>(UGameplayStatics::GetActorOfClass(GetWorld(), ACController::StaticClass()));
	l_Controller->SetMap(Info, m_SelectedDifficulty->_difficulty, SelectedMode);
	//SetVisibility(ESlateVisibility::Collapsed);
	l_Controller->SetCurrentScene("Editing");
	GetViewsController()->SetWidget("EditModeWidget");
}