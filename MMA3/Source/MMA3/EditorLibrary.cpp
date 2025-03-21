// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorLibrary.h"

AEditorLibrary::AEditorLibrary()
{
	Instance = this;
}

AEditorLibrary::~AEditorLibrary()
{
}

void AEditorLibrary::OnAudioLoaded(URuntimeAudioImporterLibrary* Importer, UImportedSoundWave* ImportedSoundWave, ERuntimeImportStatus Status) {
	if (Callback == nullptr)
		goto Finish;
	if (Callback->CanInvoke()) {
		Callback->Invoke(Importer, ImportedSoundWave, Status);
	}

Finish:

	if (Importer == nullptr) return;

	Importer->OnResult.RemoveAll(this);
	//Importer->ConditionalBeginDestroy();
}

FMapDifficulty AEditorLibrary::GetDifficultyByMapAndMode(FMapInfo p_Info, FString p_Difficulty, FString p_Mode) {
	FMapDifficulty l_Difficulty;
	bool l_Finished = false;
	for (int l_i = 0; l_i < p_Info.DifficultyBeatmapSets.Num(); l_i++) {
		if (p_Info.DifficultyBeatmapSets[l_i].Name != p_Mode) continue;

		TArray<FMapDifficulty> l_DifficultyList = p_Info.DifficultyBeatmapSets[l_i].DifficultyBeatmaps;

		for (int l_i1 = 0; l_i1 < l_DifficultyList.Num(); l_i1++) {
			if (l_DifficultyList[l_i1].Difficulty != p_Difficulty) continue;

			l_Difficulty = p_Info.DifficultyBeatmapSets[l_i].DifficultyBeatmaps[l_i1];
			return l_Difficulty;
		}
	}

	return FMapDifficulty();
}