// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C:\Program Files\Epic Games\UE_5.1\Engine\Plugins\Marketplace\VaRestPlugin\Source\VaRest\Public\VaRestJsonValue.h"
#include "C:\Program Files\Epic Games\UE_5.1\Engine\Plugins\Marketplace\VaRestPlugin\Source\VaRest\Public\VaRestSubsystem.h"
#include "Structures.generated.h"

/**
 *
 */
class MMA3_API Structures
{
public:
	Structures();
	~Structures();
};

USTRUCT()
struct FMapDifficulty {

	GENERATED_BODY()

	UPROPERTY()
		FString Difficulty;

	UPROPERTY()
		int DifficultyRank;

	UPROPERTY()
		FString BeatmapFileName;

	UPROPERTY()
		float NoteJumpMovementSpeed;

	UPROPERTY()
		float NoteJumpStartBeatOffset;

	UPROPERTY()
		FString DifficultyLabel;

	UPROPERTY()
		TArray<UVaRestJsonValue*> Contributors;

	UPROPERTY()
		TArray<FString> Requirements;

	UPROPERTY()
		TArray<FString> Warnings;

	UPROPERTY()
		TArray<FString> Informations;

	UPROPERTY()
		TArray<FString> Suggestions;

	UPROPERTY()
		TMap<FString, FLinearColor> Colors;

};

USTRUCT()
struct FDifficultyBeatmapSet {

	GENERATED_BODY()

	UPROPERTY()
		FString Name;

	UPROPERTY()
		TArray<FMapDifficulty> DifficultyBeatmaps;

};

USTRUCT()
struct MMA3_API FMapInfo {

	GENERATED_USTRUCT_BODY()

public:

	FMapInfo();

	UPROPERTY()
		FString SongName;

	UPROPERTY()
		FString SongSubName;

	UPROPERTY()
		FString SongAuthor;

	UPROPERTY()
		FString SongMapper;

	UPROPERTY()
		FString MapPath;

	UPROPERTY()
		FString AudioFileName;

	UPROPERTY()
		float BPM;

	UPROPERTY()
		float PreviewStartTime;

	UPROPERTY()
		float PreviewDuration;

	UPROPERTY()
		FString CoverImageFileName;

	UPROPERTY()
		FString EnvironmentName;

	UPROPERTY()
		TArray<FDifficultyBeatmapSet> DifficultyBeatmapSets;

	UPROPERTY()
		USoundWave* Song;

	UPROPERTY()
		UTexture2D* Cover = nullptr;

	FString GetJson();

	void FromJson(FString p_Json, FString p_MapMath);

};
