// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C:\UE\UE5_1_1\UE_5.1\Engine\Plugins\Marketplace\VaRestPlugin\Source\VaRest\Public\VaRestJsonValue.h"
#include "C:\UE\UE5_1_1\UE_5.1\Engine\Plugins\Marketplace\VaRestPlugin\Source\VaRest\Public\VaRestSubsystem.h"
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

USTRUCT()
struct MMA3_API FEnvironmentData {

	GENERATED_BODY()

public:

};

USTRUCT()
struct MMA3_API FDefaultNoodleExtensionsData {

	GENERATED_BODY()

public:

	UPROPERTY()
		bool Fake;

};

USTRUCT()
struct MMA3_API FNoteData {

	GENERATED_BODY()

public:

	UPROPERTY()
		float Beat;

	UPROPERTY()
		float Line;

	UPROPERTY()
		float Layer;

	UPROPERTY()
		int Type;

	UPROPERTY()
		int Direction;

	UPROPERTY()
		int Angle;

};

USTRUCT()
struct MMA3_API FArcData {

	GENERATED_BODY()

public:

	UPROPERTY()
		float Beat;

	UPROPERTY()
		int Color;

	UPROPERTY()
		int Line;

	UPROPERTY()
		int Layer;

	UPROPERTY()
		int HeadDirection;

	UPROPERTY()
		int HeadControlPointLengthMultiplier;

	UPROPERTY()
		float TailBeat;

	UPROPERTY()
		float TailLine;

	UPROPERTY()
		float TailLayer;

	UPROPERTY()
		float TailColor;

	UPROPERTY()
		float TailControlPointLengthMultiplier;
};

USTRUCT()
struct MMA3_API FChainData {

	GENERATED_BODY()

public:

	UPROPERTY()
		float Beat;

	UPROPERTY()
		int Color;

	UPROPERTY()
		int Line;

	UPROPERTY()
		int Layer;

	UPROPERTY()
		int Direction;

	UPROPERTY()
		int Links;

	UPROPERTY()
		int Squish;

	UPROPERTY()
		float TailBeat;

	UPROPERTY()
		int TailLine;

	UPROPERTY()
		int TailLayer;

};

USTRUCT()
struct MMA3_API FBombData {

	GENERATED_BODY()

public:

};

USTRUCT()
struct MMA3_API FWallData {

	GENERATED_BODY()

public:

	UPROPERTY()
		float Beat;

	UPROPERTY()
		int Line;

	UPROPERTY()
		int Type;

	UPROPERTY()
		float Duration;

	UPROPERTY()
		int Width;
};

USTRUCT()
struct MMA3_API FEventData {

	GENERATED_BODY()

public:

};

USTRUCT()
struct MMA3_API FBeatmapBookmark {

	GENERATED_BODY()

public:

	UPROPERTY()
		FString Name;

	UPROPERTY()
		FColor Color;

	UPROPERTY()
		float PositionInBeat;
};

USTRUCT()
struct MMA3_API FMapCustomData {

	GENERATED_BODY()

public:
	UPROPERTY()
		float LastPositionInBeat;

	UPROPERTY()
		TArray<FBeatmapBookmark> Bookmarks;
};

USTRUCT()
struct MMA3_API FMapData {

	GENERATED_BODY()

public:

	UPROPERTY()
		FString Version;

	UPROPERTY()
		TArray<FNoteData> Notes;

	UPROPERTY()
		TArray<FBombData> Bombs;

	UPROPERTY()
		TArray<FWallData> Walls;

	UPROPERTY()
		TArray<FArcData> Arcs;

	UPROPERTY()
		TArray<FChainData> Chains;

	UPROPERTY()
		TArray<FEventData> Events;

	UPROPERTY()
		FMapCustomData CustomData;

	void FromJson(FString p_Json);
};