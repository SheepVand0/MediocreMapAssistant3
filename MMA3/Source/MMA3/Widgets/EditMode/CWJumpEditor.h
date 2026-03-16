// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MMA3/Core/Structures.h"
#include "MMA3/Misc/CVaRestHelper.h"
#include "MMA3/Core/BeatmapObjects/CNote.h"
#include "MMA3/Core/CObjectsSpawner.h"
#include "CWJumpEditor.generated.h"

struct FPattern : public FJsonDataObject {

public:
	FPattern(TArray<FNoteData*> notes);

	TArray<FNoteData*> Notes;

};

/**
 * 
 */
UCLASS()
class MMA3_API UCWJumpEditor : public UUserWidget
{
	GENERATED_BODY()

protected:

	TArray<FNoteData*> LerpJumpUnique(float beat, float div, int startIndex, float& outEndBeat, int& outLastIndex);

public:

	TArray<ACNote*> DisplayedNotes;

	FPattern* CurrentPattern;


	FPattern CreateJump(int color, float startBeat, float endBeat, float div);

	FPattern CreateJumpWithLerp(int color, float startBeat, float endBeat, float div);

	UFUNCTION(BlueprintCallable)
	void ClearCurrentPattern();

	UFUNCTION(BlueprintCallable)
	void UpdateNotesPreview();

public:

	UFUNCTION(BlueprintCallable)
	void CreateJumpFromSelection(int color, float div);

	UFUNCTION(BlueprintCallable)
	void CreateLerpJumpFromSelection(float div);

	UFUNCTION(BlueprintCallable)
	void ApplyPreviewOnMap();

	//UFUNCTION(BlueprintCallable)
	//void ClearPreview();

	UFUNCTION(BlueprintCallable)
	void ShuffleSelected(int line, int layer, int maxSpace, bool allowVisionBlock, bool shuffleLine = true, bool shuffleLayer = true);

	/// <summary>
	/// Need to optimize ts,
	/// for positive numbers only
	/// </summary>
	/// <param name="value"></param>
	/// <param name="mod"></param>
	/// <returns></returns>
	float FloatMod(float value, float mod);
};
