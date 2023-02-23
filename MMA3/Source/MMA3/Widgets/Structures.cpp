// Fill out your copyright notice in the Description page of Project Settings.


#include "Structures.h"
#include "WidgetUtils.h"
#include "RenderUtils.h"
#include "Engine/Texture2D.h"
#include "Containers/Array.h"
#include "MMA3/VaRestJsonObjectUtils.h"

Structures::Structures()
{

}

Structures::~Structures()
{

}

FMapInfo::FMapInfo() {}

FString FMapInfo::GetJson() {
	return FString("");
}

void FMapInfo::FromJson(FString p_Json, FString p_MapPath) {

	UVaRestJsonObject* l_Object = GEngine->GetEngineSubsystem<UVaRestSubsystem>()->ConstructVaRestJsonObject();

	l_Object->DecodeJson(p_Json);

	SongName = VaRestJsonObjectUtils::GetStringField(l_Object, "_songName");
	SongSubName = VaRestJsonObjectUtils::GetStringField(l_Object, "_songSubName");
	SongAuthor = VaRestJsonObjectUtils::GetStringField(l_Object, "_songAuthorName");
	SongMapper = VaRestJsonObjectUtils::GetStringField(l_Object, "_levelAuthorName");
	MapPath = p_MapPath;
	AudioFileName = VaRestJsonObjectUtils::GetStringField(l_Object, "_songFileName");
	CoverImageFileName = VaRestJsonObjectUtils::GetStringField(l_Object, "_coverImageFileName");
	EnvironmentName = VaRestJsonObjectUtils::GetStringField(l_Object, "_environmentName");
	BPM = VaRestJsonObjectUtils::GetNumberField(l_Object, "_beatsPerMinute");

	TArray<UVaRestJsonValue*> l_DifficultyBeatmapSets = VaRestJsonObjectUtils::GetValueArrayField(l_Object, "_difficultyBeatmapSets");

	for (int l_i = 0; l_i < l_DifficultyBeatmapSets.Num(); l_i++) {
		FDifficultyBeatmapSet l_BeatmapSet = FDifficultyBeatmapSet{};
		l_BeatmapSet.Name = VaRestJsonObjectUtils::GetStringField(Cast<UVaRestJsonValue>(l_DifficultyBeatmapSets[l_i])->AsObject(), "_beatmapCharacteristicName");

		TArray<FMapDifficulty> l_Difficulties = TArray<FMapDifficulty>();
		TArray<UVaRestJsonValue*> l_SerializedDifficulties = VaRestJsonObjectUtils::GetValueArrayField(l_DifficultyBeatmapSets[l_i]->AsObject(), "_difficultyBeatmaps");

		for (int l_i1 = 0; l_i1 < l_SerializedDifficulties.Num(); l_i1++) {
			FMapDifficulty l_Difficulty = FMapDifficulty{};
			UVaRestJsonValue* l_BeatmapDifficultyValue = l_SerializedDifficulties[l_i1];
			UVaRestJsonObject* l_BeatmapDifficultyObject = l_BeatmapDifficultyValue->AsObject();
			l_Difficulty.Difficulty = VaRestJsonObjectUtils::GetStringField(l_BeatmapDifficultyObject, "_difficulty");
			l_Difficulty.DifficultyRank = VaRestJsonObjectUtils::GetIntegerField(l_BeatmapDifficultyObject, "_difficultyRank");
			l_Difficulty.NoteJumpMovementSpeed = VaRestJsonObjectUtils::GetNumberField(l_BeatmapDifficultyObject, "_noteJumpMovementSpeed");
			l_Difficulty.NoteJumpStartBeatOffset = VaRestJsonObjectUtils::GetNumberField(l_BeatmapDifficultyObject, "_noteJumpStartBeatOffset");

			UVaRestJsonValue* l_CustomDataValue = l_BeatmapDifficultyObject->GetField("_customData");
			if (l_CustomDataValue == nullptr) continue;
			UVaRestJsonObject* l_CustomData = l_CustomDataValue->AsObject();

			l_Difficulty.DifficultyLabel = VaRestJsonObjectUtils::GetStringField(l_BeatmapDifficultyObject, "_difficultyLabel");
			l_Difficulty.Requirements = VaRestJsonObjectUtils::GetStringArrayField(l_BeatmapDifficultyObject, "_requirements");
			l_Difficulty.Warnings = VaRestJsonObjectUtils::GetStringArrayField(l_BeatmapDifficultyObject, "_warnings");
			l_Difficulty.Informations = VaRestJsonObjectUtils::GetStringArrayField(l_BeatmapDifficultyObject, "_information");
			l_Difficulty.Suggestions = VaRestJsonObjectUtils::GetStringArrayField(l_BeatmapDifficultyObject, "_suggestions");

			l_Difficulties.Add(l_Difficulty);
		}

		l_BeatmapSet.DifficultyBeatmaps = l_Difficulties;

		DifficultyBeatmapSets.Add(l_BeatmapSet);
	}

}