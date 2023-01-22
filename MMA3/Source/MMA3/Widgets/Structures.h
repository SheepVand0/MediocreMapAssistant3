// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
struct MMA3_API FMapInfo {

	GENERATED_USTRUCT_BODY()

public:

	FMapInfo();

	FMapInfo(FString p_SongName, FString p_SongSubName, FString p_SongAuthor, FString p_SongMapper, FString p_MapPath, FString p_AudioFileName);

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
		TArray<UVaRestJsonValue> Contributors;

	///Finish Data list from info.dat of a map

	UPROPERTY()
		USoundWave* Song;

};
