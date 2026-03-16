// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VaRest\Public\VaRestJsonValue.h"
#include "VaRest\Public\VaRestSubsystem.h"
#include "RuntimeAudioImporter/Public/Sound/ImportedSoundWave.h"
#include "MMA3/FastLog.h"
#include "MMA3/Misc/CVaRestHelper.h"
#include "Structures.generated.h"

UENUM(BlueprintType)
enum class EDefaultRequirement : uint8 {
	NoodleExtensions,
	Chroma,
	MappingExtensions
};

class MMA3_API Structures
{
public:
	Structures();
	~Structures();

	static FString DefaultRequirementToString(EDefaultRequirement requirement);
};

struct FMapDifficultyCustomData : public FJsonDataObject {

public:

	FMapDifficultyCustomData();

	FString DifficultyLabel;


	TArray<FJsonDataObject*> Contributors;


	TArray<FString> Requirements;


	TArray<FString> Warnings;


	TArray<FString> Informations;


	TArray<FString> Suggestions;
};

struct FMapDifficulty : public FJsonDataObject {

public:

	FMapDifficulty();

	FString _difficulty;

	float _difficultyRank;

	FString _beatmapFilename;

	float _noteJumpMovementSpeed;

	float _noteJumpStartBeatOffset;

	FMapDifficultyCustomData* DifficultyCustomData;

	TMap<FString, FLinearColor> Colors;

	bool HasRequirement(EDefaultRequirement requirement);

	bool HasRequirement(FString requirement);
};

struct FDifficultyBeatmapSet : public FJsonDataObject {


public:
	FDifficultyBeatmapSet();

	FString Name;

	TArray<FMapDifficulty*> DifficultyBeatmaps;

};

struct MMA3_API FMapInfo : public FJsonDataObject {

public:

	explicit FMapInfo();

	FString _songName;

	FString _songSubName;

	FString _songAuthorName;

	FString _levelAuthorName;

	FString MapPath;

	FString _songFileName;

	float _beatsPerMinute;

	float _previewStartTime;

	float _previewDuration;

	float Shuffle;

	int SufflePeriod;

	FString _coverImageFileName;

	FString _environmentName;

	FString _allDirectionsEnvironmentName;

	TArray<FDifficultyBeatmapSet*> DifficultyBeatmapSets;

	TObjectPtr<UImportedSoundWave> Song;

	/// <summary>
	/// Used for visualization
	/// </summary>
	uint8* SongPCMData;

	int32 PCMNumberOfValues;

	int32 NeededSamples;

	TObjectPtr<UTexture2D> Cover = nullptr;

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

struct MMA3_API FNoteData : public FJsonDataObject {

public:

	FNoteData() : Direction(0), Angle(0) {
		MARK_JSON_FLOAT_ALIASES(Beat, "b");
		MARK_JSON_FLOAT_ALIASES(Beat, "_time");
		//MARK_JSON_FLOAT_ALIASES(Beat, "b");

		MARK_JSON_FLOAT_ALIASES(Line, "x");
		MARK_JSON_FLOAT_ALIASES(Line, "i");
		MARK_JSON_FLOAT_ALIASES(Line, "_lineIndex");
		

		MARK_JSON_FLOAT_ALIASES(Layer, "y");
		MARK_JSON_FLOAT_ALIASES(Layer, "j");
		MARK_JSON_FLOAT_ALIASES(Layer, "_lineLayer");
		

		MARK_JSON_FLOAT_ALIASES(Type, "c");
		MARK_JSON_FLOAT_ALIASES(Type, "_type");
		
		MARK_JSON_FLOAT_ALIASES(Direction, "d");
		MARK_JSON_FLOAT_ALIASES(Direction, "_cutDirection");

		MARK_JSON_FLOAT_ALIASES(Angle, "a");
		MARK_JSON_FLOAT_ALIASES(Angle, "_angle");
	}

	float Beat;

	float Line;

	float Layer;

	float Type;

	float Direction;

	float Angle;


};

FORCEINLINE bool operator < (const FNoteData& x1, const FNoteData& x2) {
	return x1.Beat < x2.Beat;
}

FORCEINLINE bool operator > (const FNoteData& x1, const FNoteData& x2) {
	return x1.Beat > x2.Beat;
}

struct MMA3_API FArcData : public FJsonDataObject {

public:

	FArcData();

	float Beat;

	int Color;

	int Line;

	int Layer;

	int HeadDirection;

	float HeadControlPointLengthMultiplier;

	float TailBeat;

	float TailLine;

	float TailLayer;

	float TailDirection;

	float TailControlPointLengthMultiplier;

	float m;
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

struct MMA3_API FWallData : public FJsonDataObject {

public:

	FWallData();

	float Beat;

	float Line;

	float Type;

	float Layer;

	float Duration;

	float Width;

	float Height;

	virtual void PostParse() override;
};

FORCEINLINE bool operator == (const FWallData& x1, const FWallData& x2) {
	bool l_Value = x1.Beat == x2.Beat && x1.Duration == x2.Duration;
	return l_Value;
}

FORCEINLINE bool operator == (FWallData& x1, FWallData& x2) {
	bool l_Value = x1.Beat == x2.Beat && x1.Duration == x2.Duration;
	return l_Value;
}

FORCEINLINE bool operator < (FWallData& x1, FWallData& x2) {
	return x1.Beat < x2.Beat;
}

FORCEINLINE bool operator < (const FWallData& x1, const FWallData& x2) {
	return x1.Beat < x2.Beat;
}

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

struct MMA3_API FMapData : public FJsonDataObject {

public:

	FMapData() {
		MARK_JSON_ARRAY_ALIASES(_notes, "colorNotes");
		MARK_JSON_ARRAY(_notes);
		
		MARK_JSON_ARRAY(_bombs);
		
		MARK_JSON_ARRAY_ALIASES(_walls, "obstacles");
		MARK_JSON_ARRAY_ALIASES(_walls, "_obstacles");

		MARK_JSON_STRING_ALIASES(Version, "version");
		MARK_JSON_STRING_ALIASES(Version, "_version");
		
		MARK_JSON_ARRAY_ALIASES(_arcs, "sliders");
	}

	FString Version;

	TArray<FNoteData*> _notes;

	TArray<FNoteData*> _bombs;

	TArray<FWallData*> _walls;

	TArray<FArcData*> _arcs;

	TArray<FChainData> Chains;

	TArray<FEventData> Events;

	FMapCustomData CustomData;

	virtual void PreParse() override;

	void FromJson(FString p_Json);

	UVaRestJsonObject* ToJson();
};