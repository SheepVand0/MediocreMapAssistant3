// Fill out your copyright notice in the Description page of Project Settings.


#include "Structures.h"
#include "WidgetUtils.h"
#include "RenderUtils.h"
#include "Engine/Texture2D.h"
#include "Containers/Array.h"

Structures::Structures()
{

}

Structures::~Structures()
{

}

FMapInfo::FMapInfo() {}

FString FMapInfo::GetJson() {

}

void FMapInfo::FromJson(FString p_Json, FString p_MapPath) {

	UVaRestJsonObject* l_Object = UVaRestSubsystem().ConstructVaRestJsonObject();

	l_Object->DecodeJson(p_Json);

	SongName = l_Object->GetStringField("_songName");
	SongSubName = l_Object->GetStringField("_songSubName");
	SongAuthor = l_Object->GetStringField("_songAuthor");
	SongMapper = l_Object->GetStringField("_levelAuthorName");
	MapPath = p_MapPath;
	AudioFileName = l_Object->GetStringField("_songFileName");
	CoverImageFileName = l_Object->GetStringField("_coverImageFileName");
	EnvironmentName = l_Object->GetStringField("_environmentName");

	TArray<UVaRestJsonValue*> l_DifficultyBeatmapSets = l_Object->GetArrayField("_difficultyBeatmapSets");

	for (int l_i = 0; l_i < l_DifficultyBeatmapSets.Num(); l_i++) {
		FDifficultyBeatmapSet l_BeatmapSet = FDifficultyBeatmapSet{};
		l_BeatmapSet.Name = Cast<UVaRestJsonValue>(l_DifficultyBeatmapSets[l_i])->AsObject()->GetStringField("_beatmapCharacteristicName");

		TArray<FMapDifficulty> l_Difficulties = TArray<FMapDifficulty>();
		TArray<UVaRestJsonValue*> l_SerializedDifficulties = l_DifficultyBeatmapSets[l_i]->AsObject()->GetArrayField("_difficultyBeatmaps");

		for (int l_i = 0; l_i < l_SerializedDifficulties.Num(); l_i++) {
			FMapDifficulty l_Difficulty = {};
			UVaRestJsonObject* l_Object = l_SerializedDifficulties[l_i]->AsObject();
			l_Difficulty.Difficulty = l_Object->GetStringField("_difficulty");
			l_Difficulty.DifficultyRank = l_Object->GetIntegerField("_difficultyRank");
			l_Difficulty.NoteJumpMovementSpeed = l_Object->GetNumberField("_noteJumpMovementSpeed");
			l_Difficulty.NoteJumpStartBeatOffset = l_Object->GetNumberField("_noteJumpStartBeatOffset");

			UVaRestJsonValue* l_CustomDataValue = l_Object->GetField("_customData");
			UVaRestJsonObject* l_CustomData = l_CustomDataValue->AsObject();

			l_Difficulty.DifficultyLabel = l_CustomData->GetStringField("_difficultyLabel");
			l_Difficulty.Requirements = l_CustomData->GetStringArrayField("_requirements");
			l_Difficulty.Warnings = l_CustomData->GetStringArrayField("_warnings");
			l_Difficulty.Informations = l_CustomData->GetStringArrayField("_information");
			l_Difficulty.Suggestions = l_CustomData->GetStringArrayField("_suggestions");

			l_Difficulties.Add(l_Difficulty);
		}

		l_BeatmapSet.DifficultyBeatmaps = l_Difficulties;

		DifficultyBeatmapSets.Add(l_BeatmapSet);
	}

}