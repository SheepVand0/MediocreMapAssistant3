// Fill out your copyright notice in the Description page of Project Settings.


#include "MapDetailsWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MMA3/Core/C_Controller.h"
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

void UMapDetailsWidget::NativeConstruct() {
	EventOnDifficultySelected.AddDynamic(this, &UMapDetailsWidget::OnDifficultySelected);
	CEditButton->OnClicked.AddDynamic(this, &UMapDetailsWidget::OnEditButtonClicked);
	/*RuntimeAudioImporter = URuntimeAudioImporterLibrary::CreateRuntimeAudioImporter();
	RuntimeAudioImporter->OnResult.AddDynamic(this, &UMapDetailsWidget::FinishedLoadingAudio);*/
	OnMapCellsFinished.AddDynamic(this, &UMapDetailsWidget::OnMapCellsFnisihedToBeAddCallback);

	Instance = this;

	SetVisibility(ESlateVisibility::Collapsed);
}

void UMapDetailsWidget::SetMap(FMapInfo p_Map) {

	Info = p_Map;

	MapName->SetText(FText::FromString(p_Map.SongName));
	MapSubName->SetText(FText::FromString(p_Map.SongSubName));
	AuthorName->SetText(FText::FromString(p_Map.SongAuthor));
	Mapper->SetText(FText::FromString(p_Map.SongMapper));
	SongPath->SetText(FText::FromString(p_Map.AudioFileName));
	CoverFile->SetText(FText::FromString(p_Map.CoverImageFileName));
	BPM->SetValue(p_Map.BPM);
	PreviewDuration->SetValue(p_Map.PreviewDuration);
	PreviewTime->SetValue(p_Map.PreviewStartTime);

	UpdateDifficultyList("Standard");

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

void UMapDetailsWidget::UpdateDifficultyList(FString p_Mode) {

	SelectedMode = p_Mode;
	FDifficultyBeatmapSet l_SelectedSet;
	for (int l_i = 0; l_i < Info.DifficultyBeatmapSets.Num(); l_i++) {
		if (Info.DifficultyBeatmapSets[l_i].Name != p_Mode) continue;

		l_SelectedSet = Info.DifficultyBeatmapSets[l_i];
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

	if (l_SelectedSet.DifficultyBeatmaps.Num() != 0)
		OnDifficultySelected(l_SelectedSet.DifficultyBeatmaps[0].Difficulty);

}

void UMapDetailsWidget::OnMapCellsFnisihedToBeAddCallback() {
	if (ListBeatmapDifficultiesSerialized.Num() > 0)
		OnDifficultySelected(ListBeatmapDifficultiesSerialized[0]);
}

void UMapDetailsWidget::UpdateCover() {

	FString l_Path = Info.MapPath + "\\" + Info.CoverImageFileName;
	Info.Cover = FImageUtils::ImportFileAsTexture2D(*l_Path);

	if (Info.Cover != nullptr) {
		CoverPreview->SetBrushFromTexture(Info.Cover);
		CoverNotFound->SetVisibility(ESlateVisibility::Collapsed);
	}
	else {
		CoverNotFound->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMapDetailsWidget::OnDifficultySelected(FString SerializedDifficulty) {
	//GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::White, FString(SerializedDifficulty));
	SetDifficultyPropertiesEnable(true);
	FMapDifficulty l_Difficulty = AEditorLibrary::GetDifficultyByMapAndMode(Info, SerializedDifficulty, SelectedMode);
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

void UMapDetailsWidget::OnEditButtonClicked() {
	AEditorLibrary::LoadAudio(Info.MapPath + "\\" + Info.AudioFileName, ERuntimeAudioFormat::OggVorbis, (new SMethodPtr(this, &UMapDetailsWidget::FinishedLoadingAudio))->GetPointer());
}

void UMapDetailsWidget::FinishedLoadingAudio(URuntimeAudioImporterLibrary* Importer, UImportedSoundWave* ImportedSoundWave, ERuntimeImportStatus Status) {
	if (ImportedSoundWave == nullptr || Status != ERuntimeImportStatus::SuccessfulImport) {
		UE_LOG(LogTemp, Error, TEXT("Sound File not found"));
		GEngine->AddOnScreenDebugMessage(3, 1, FColor::Red, FString("Error Loading Audio"));
		return;
	}
	//ImportedSoundWave->OnGeneratePCMData.AddDynamic(AC_Controller::Instance, &AC_Controller::);

	Info.Song = ImportedSoundWave;
	int l_Samples = GEngine->GetMainAudioDevice().GetAudioDevice()->GetSampleRate();

	TArray<uint8> l_SongPCMData;
	Info.NeededSamples = ImportedSoundWave->OnGeneratePCMAudio(l_SongPCMData, l_Samples);
	Info.SongPCMData = l_SongPCMData.GetData();
	Info.PCMNumberOfValues = l_SongPCMData.Num();

	TArrayView<float> l_PCMDataView = ImportedSoundWave->GetPCMBuffer().PCMData.GetView();
	UE_LOG(LogTemp, Display, TEXT("PCM Data count: %d"), l_PCMDataView.Num());
	UE_LOG(LogTemp, Display, TEXT("PCM Data middle value: %f"), l_PCMDataView[l_PCMDataView.Num() / 2]);

	/// TODO : Move audio load in other class

	GEngine->AddOnScreenDebugMessage(0, 10, FColor::White, FString("Editing"));
	AC_Controller* l_Controller = Cast<AC_Controller>(UGameplayStatics::GetActorOfClass(GetWorld(), AC_Controller::StaticClass()));
	l_Controller->GenerateGrid(Info, m_SelectedDifficulty.Difficulty, SelectedMode);
	SetVisibility(ESlateVisibility::Collapsed);
	l_Controller->SetCurrentScene("Editing");
}