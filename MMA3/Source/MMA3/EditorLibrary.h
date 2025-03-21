// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMA3/Misc/SActionPtr.h"
#include "RuntimeAudioImporter\Public\RuntimeAudioImporterLibrary.h"
#include "RuntimeAudioImporter\Public\RuntimeAudioImporterTypes.h"
#include "RuntimeAudioImporter\Public\Sound\ImportedSoundWave.h"
#include "MMA3/Widgets/Structures.h"
#include "EditorLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MMA3_API AEditorLibrary : public AActor
{

	GENERATED_BODY()

public:
	AEditorLibrary();
	~AEditorLibrary();

	static AEditorLibrary* Instance;

	UPROPERTY()
		URuntimeAudioImporterLibrary* RuntimeAudioImporter = nullptr;

protected:

	UFUNCTION()
	void OnAudioLoaded(URuntimeAudioImporterLibrary* Importer, UImportedSoundWave* ImportedSoundWave, ERuntimeImportStatus Status);

	SActionPtrBase<void, URuntimeAudioImporterLibrary*, UImportedSoundWave*, ERuntimeImportStatus>* Callback;

	void _LoadAudio(FString path, ERuntimeAudioFormat format, SActionPtrBase<void, URuntimeAudioImporterLibrary*, UImportedSoundWave*, ERuntimeImportStatus>* callback) {
		if (RuntimeAudioImporter == nullptr)
			RuntimeAudioImporter = URuntimeAudioImporterLibrary::CreateRuntimeAudioImporter();
		Callback = callback;
		RuntimeAudioImporter->OnResult.AddDynamic(this, &AEditorLibrary::OnAudioLoaded);
		RuntimeAudioImporter->ImportAudioFromFile(path, format);
	}

public:

	static void LoadAudio(FString path, ERuntimeAudioFormat format, SActionPtrBase<void, URuntimeAudioImporterLibrary*, UImportedSoundWave*, ERuntimeImportStatus>* callback) {
		Instance->_LoadAudio(path, format, callback);
	}

	static FMapDifficulty GetDifficultyByMapAndMode(FMapInfo p_Info, FString p_Difficulty, FString p_Mode);

};

AEditorLibrary* AEditorLibrary::Instance = nullptr;