// Fill out your copyright notice in the Description page of Project Settings.


#include "Structures.h"
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

FString Structures::DefaultRequirementToString(EDefaultRequirement requirement)
{
	switch (requirement) {
		case EDefaultRequirement::MappingExtensions:
			return FString("Mapping Extensions");
		case EDefaultRequirement::NoodleExtensions:
			return FString("Noodle Extensions");
		case EDefaultRequirement::Chroma:
			return FString("Chroma");
		default: return FString();
	}
}

FMapInfo::FMapInfo() {
	MARK_JSON_STRING(_songName);
	MARK_JSON_STRING(_songSubName);
	MARK_JSON_STRING(_songAuthorName);
	MARK_JSON_STRING(_levelAuthorName);
	MARK_JSON_STRING(_songFileName);
	MARK_JSON_STRING(_coverImageFileName);
	MARK_JSON_STRING(_environmentName);
	MARK_JSON_FLOAT(_beatsPerMinute);
	MARK_JSON_FLOAT(_previewStartTime);
	MARK_JSON_FLOAT(_previewDuration);
	MARK_JSON_STRING(_allDirectionsEnvironmentName);

	MARK_JSON_ARRAY_ALIASES(DifficultyBeatmapSets, "_difficultyBeatmapSets");
}

void FMapInfo::FromJson(FString p_Json, FString p_MapPath) {

	UVaRestJsonObject* l_Object = GEngine->GetEngineSubsystem<UVaRestSubsystem>()->ConstructVaRestJsonObject();

	l_Object->DecodeJson(p_Json);

	Parse(l_Object);

	MapPath = p_MapPath;

	//TArray<UVaRestJsonValue*> l_DifficultyBeatmapSets = VaRestJsonObjectUtils::GetValueArrayField(l_Object, "_difficultyBeatmapSets");

	//for (int l_i = 0; l_i < l_DifficultyBeatmapSets.Num(); l_i++) {
	//	FDifficultyBeatmapSet l_BeatmapSet = FDifficultyBeatmapSet{};
	//	l_BeatmapSet.Name = VaRestJsonObjectUtils::GetStringField(Cast<UVaRestJsonValue>(l_DifficultyBeatmapSets[l_i])->AsObject(), "_beatmapCharacteristicName");

	//	TArray<FMapDifficulty> l_Difficulties = TArray<FMapDifficulty>();
	//	TArray<UVaRestJsonValue*> l_SerializedDifficulties = VaRestJsonObjectUtils::GetValueArrayField(l_DifficultyBeatmapSets[l_i]->AsObject(), "_difficultyBeatmaps");

	//	for (int l_i1 = 0; l_i1 < l_SerializedDifficulties.Num(); l_i1++) {
	//		FMapDifficulty l_Difficulty = FMapDifficulty{};
	//		UVaRestJsonValue* l_BeatmapDifficultyValue = l_SerializedDifficulties[l_i1];
	//		UVaRestJsonObject* l_BeatmapDifficultyObject = l_BeatmapDifficultyValue->AsObject();
	//		/*l_Difficulty._difficulty = VaRestJsonObjectUtils::GetStringField(l_BeatmapDifficultyObject, "_difficulty");
	//		l_Difficulty._difficultyRank = VaRestJsonObjectUtils::GetIntegerField(l_BeatmapDifficultyObject, "_difficultyRank");
	//		l_Difficulty._noteJumpMovementSpeed = VaRestJsonObjectUtils::GetNumberField(l_BeatmapDifficultyObject, "_noteJumpMovementSpeed");
	//		l_Difficulty._noteJumpStartBeatOffset = VaRestJsonObjectUtils::GetNumberField(l_BeatmapDifficultyObject, "_noteJumpStartBeatOffset");
	//		l_Difficulty._beatmapFilename = VaRestJsonObjectUtils::GetStringField(l_BeatmapDifficultyObject, "_beatmapFilename");*/

	//		l_Difficulty.Parse(l_BeatmapDifficultyObject);

	//		UVaRestJsonValue* l_CustomDataValue = l_BeatmapDifficultyObject->GetField("_customData");
	//		if (l_CustomDataValue == nullptr) continue;
	//		UVaRestJsonObject* l_CustomData = l_CustomDataValue->AsObject();

	//		l_Difficulty.DifficultyLabel = VaRestJsonObjectUtils::GetStringField(l_CustomData, "_difficultyLabel");
	//		l_Difficulty.Requirements = VaRestJsonObjectUtils::GetStringArrayField(l_CustomData, "_requirements");
	//		l_Difficulty.Warnings = VaRestJsonObjectUtils::GetStringArrayField(l_CustomData, "_warnings");
	//		l_Difficulty.Informations = VaRestJsonObjectUtils::GetStringArrayField(l_CustomData, "_informations");
	//		l_Difficulty.Suggestions = VaRestJsonObjectUtils::GetStringArrayField(l_CustomData, "_suggestions");

	//		l_Difficulties.Add(l_Difficulty);
	//	}

	//	l_BeatmapSet.DifficultyBeatmaps = l_Difficulties;

	//	DifficultyBeatmapSets.Add(l_BeatmapSet);
	//}
}

FMapDifficulty::FMapDifficulty()
{
	MARK_JSON_STRING(_difficulty);
	MARK_JSON_FLOAT(_difficultyRank);
	MARK_JSON_FLOAT(_noteJumpMovementSpeed);
	MARK_JSON_FLOAT_ALIASES(_noteJumpStartBeatOffset, "_noteJumpStartBeatOffset");
	MARK_JSON_STRING(_beatmapFilename);

	MARK_JSON_OBJECT_ALIASES(DifficultyCustomData, "_customData", FMapDifficultyCustomData);
}

FString FMapInfo::GetJson() {
	/*UVaRestJsonObject* l_MainObject = GEngine->GetEngineSubsystem<UVaRestSubsystem>()->ConstructVaRestJsonObject();

	l_MainObject->SetStringField("_version", "2.1.0");
	l_MainObject->SetStringField("_songName", _songName);
	l_MainObject->SetStringField("_songSubName", _songSubName);
	l_MainObject->SetStringField("_songAuthorName", _songAuthorName);
	l_MainObject->SetStringField("_levelAuthorName", _levelAuthorName);
	l_MainObject->SetNumberField("_beatsPerMinute", _beatsPerMinute);
	
	l_MainObject->SetNumberField("_previewStartTime", _previewStartTime);
	l_MainObject->SetNumberField("_previewDuration", _previewDuration);
	l_MainObject->SetNumberField("_songTimeOffset", 0);
	l_MainObject->SetNumberField("_shuffle", 0);
	l_MainObject->SetNumberField("_shufflePeriod", 0.5);
	l_MainObject->SetStringField("_coverImageFilename", _coverImageFileName);
	l_MainObject->SetStringField("_songFilename", _songFileName);
	l_MainObject->SetStringField("_environmentName", _environmentName);
	l_MainObject->SetStringField("_allDirectionsEnvironmentName", "GlassDesertEnvironment");

	l_MainObject->SetObjectArrayField("_environmentNames", TArray<UVaRestJsonObject*>());
	l_MainObject->SetObjectArrayField("_colorSchemes", TArray<UVaRestJsonObject*>());
	
	TArray<UVaRestJsonObject*> l_DifficultyBeatmapSets;
	for (int l_i = 0; l_i < DifficultyBeatmapSets.Num(); l_i++) {
		UVaRestJsonObject* l_Value = GEngine->GetEngineSubsystem<UVaRestSubsystem>()->ConstructVaRestJsonObject();
		l_Value->SetStringField("_beatmapCharacteristicName", DifficultyBeatmapSets[l_i].Name);

		TArray<UVaRestJsonObject*> l_Difficulties;
		for (int l_d = 0; l_d < DifficultyBeatmapSets[l_i].DifficultyBeatmaps.Num(); l_d++) {
			auto l_Difficulty = DifficultyBeatmapSets[l_i].DifficultyBeatmaps[l_d];

			UVaRestJsonObject* l_Diff = GEngine->GetEngineSubsystem<UVaRestSubsystem>()->ConstructVaRestJsonObject();
			l_Diff->SetStringField("_difficulty", l_Difficulty._difficulty);
			l_Diff->SetIntegerField("_difficultyRank", l_Difficulty._difficultyRank);
			l_Diff->SetNumberField("_noteJumpMovementSpeed", l_Difficulty._noteJumpMovementSpeed);
			l_Diff->SetNumberField("_noteJumpStartBeatOffset", l_Difficulty._noteJumpStartBeatOffset);
			l_Diff->SetStringField("_beatmapFilename", l_Difficulty._beatmapFilename);
			l_Diff->SetIntegerField("_beatmapColorSchemeIdx", 0);
			l_Diff->SetIntegerField("_environmentNameIdx", 0);

			UVaRestJsonObject* l_CustomDataObj = GEngine->GetEngineSubsystem<UVaRestSubsystem>()->ConstructVaRestJsonObject();
			l_CustomDataObj->SetStringField("_difficultyLabel", l_Difficulty.DifficultyLabel);
			l_CustomDataObj->SetStringArrayField("_requirements", l_Difficulty.Requirements);
			l_CustomDataObj->SetStringArrayField("_warnings", l_Difficulty.Warnings);
			l_CustomDataObj->SetStringArrayField("_informations", l_Difficulty.Informations);
			l_CustomDataObj->SetStringArrayField("_suggestions", l_Difficulty.Suggestions);

			l_Diff->SetObjectField("_customData", l_CustomDataObj);
			l_Difficulties.Add(l_Diff);
		}

		l_Value->SetObjectArrayField("_difficultyBeatmaps", l_Difficulties);
		l_DifficultyBeatmapSets.Add(l_Value);
	}

	l_MainObject->SetObjectArrayField("_difficultyBeatmapSets", l_DifficultyBeatmapSets);

	FString l_Result = l_MainObject->EncodeJson();

	FFileHelper::SaveStringToFile(l_Result, TEXT("C:\\Users\\lphel\\Documents\\GitHub\\MediocreMapAssistant3\\MMA3\\test_see.dat"), FFileHelper::EEncodingOptions::ForceUTF8);*/

	return Serialize();
}

void FMapData::PreParse()
{
	Version = "3.3.0";
}

void FMapData::FromJson(FString p_Json) {
	UVaRestJsonObject* l_Object = GEngine->GetEngineSubsystem<UVaRestSubsystem>()->ConstructVaRestJsonObject();

	l_Object->DecodeJson(p_Json);

	Parse(l_Object);
}

UVaRestJsonObject* FMapData::ToJson()
{
	UVaRestJsonObject* l_JsonObject = GEngine->GetEngineSubsystem<UVaRestSubsystem>()->ConstructVaRestJsonObject();

	/*if (Version[0] == '2') {
		if (Version[2] == '2' || Version[2] == '6') {

			l_JsonObject->SetStringField("_version", Version);

			TArray<UVaRestJsonObject*> l_Notes;

			for (int l_i = 0; l_i < _notes.Num(); l_i++) {
				FNoteData l_Note = *_notes[l_i];
				UVaRestJsonObject* l_NoteObj = GEngine->GetEngineSubsystem<UVaRestSubsystem>()->ConstructVaRestJsonObject();
				l_NoteObj->SetNumberField("_time", l_Note.Beat);
				l_NoteObj->SetIntegerField("_lineIndex", l_Note.Line);
				l_NoteObj->SetIntegerField("_lineLayer", l_Note.Layer);
				l_NoteObj->SetIntegerField("_type", l_Note.Type);
				l_NoteObj->SetIntegerField("_cutDirection", l_Note.Direction);
				l_Notes.Add(l_NoteObj);
			}

			TArray<UVaRestJsonObject*> l_Walls;

			for (int l_i = 0; l_i < _walls.Num(); l_i++) {
				FWallData l_Wall = *_walls[l_i];
				UVaRestJsonObject* l_WallObj = GEngine->GetEngineSubsystem<UVaRestSubsystem>()->ConstructVaRestJsonObject();
				l_WallObj->SetNumberField("_time", l_Wall.Beat);
				l_WallObj->SetIntegerField("_lineIndex", l_Wall.Line);
				l_WallObj->SetIntegerField("_type", l_Wall.Layer == 0 ? 0 : 1);
				l_WallObj->SetIntegerField("_width", l_Wall.Width);
				l_WallObj->SetIntegerField("_duration", l_Wall.Duration);
				l_Walls.Add(l_WallObj);
			}

			l_JsonObject->SetObjectArrayField("_notes", l_Notes);
			l_JsonObject->SetObjectArrayField("_events", TArray<UVaRestJsonObject*>());
			l_JsonObject->SetObjectArrayField("_obstacles", l_Walls);
		}

		return l_JsonObject;
	}

	if (Version[0] == '3') {
		l_JsonObject->SetStringField("_version", Version);

		TArray<UVaRestJsonObject*> l_Notes;

		for (int l_i = 0; l_i < _notes.Num(); l_i++) {
			FNoteData l_Note = *_notes[l_i];
			UVaRestJsonObject* l_NoteObj = GEngine->GetEngineSubsystem<UVaRestSubsystem>()->ConstructVaRestJsonObject();
			l_NoteObj->SetNumberField("b", l_Note.Beat);
			l_NoteObj->SetIntegerField("x", l_Note.Line);
			l_NoteObj->SetIntegerField("y", l_Note.Layer);
			l_NoteObj->SetIntegerField("c", l_Note.Type);
			l_NoteObj->SetIntegerField("d", l_Note.Direction);
			l_NoteObj->SetIntegerField("a", l_Note.Direction);
			l_Notes.Add(l_NoteObj);
		}

		TArray<UVaRestJsonObject*> l_Walls;
		for (int l_i = 0; l_i < _walls.Num(); l_i++) {
			FWallData l_Wall = *_walls[l_i];
			UVaRestJsonObject* l_WallObj = GEngine->GetEngineSubsystem<UVaRestSubsystem>()->ConstructVaRestJsonObject();
			l_WallObj->SetNumberField("b", l_Wall.Beat);
			l_WallObj->SetIntegerField("x", l_Wall.Line);
			l_WallObj->SetIntegerField("y", l_Wall.Layer);
			l_WallObj->SetIntegerField("w", l_Wall.Width);
			l_WallObj->SetIntegerField("h", l_Wall.Height);
			l_WallObj->SetIntegerField("d", l_Wall.Duration);
			l_Walls.Add(l_WallObj);
		}

		TArray<UVaRestJsonObject*> l_Sliders;
		for (int l_i = 0; l_i < _arcs.Num(); l_i++) {
			FArcData l_Arc = *_arcs[l_i];
			UVaRestJsonObject* l_ArcObj = GEngine->GetEngineSubsystem<UVaRestSubsystem>()->ConstructVaRestJsonObject();
			l_ArcObj->SetNumberField("b", l_Arc.Beat);
			l_ArcObj->SetNumberField("c", l_Arc.Color);
			l_ArcObj->SetNumberField("x", l_Arc.Line);
			l_ArcObj->SetNumberField("y", l_Arc.Layer);
			l_ArcObj->SetNumberField("d", l_Arc.HeadDirection);
			l_ArcObj->SetNumberField("mu", l_Arc.HeadControlPointLengthMultiplier);
			l_ArcObj->SetNumberField("tb", l_Arc.TailBeat);
			l_ArcObj->SetNumberField("tx", l_Arc.TailLine);
			l_ArcObj->SetNumberField("ty", l_Arc.TailLayer);
			l_ArcObj->SetNumberField("tc", l_Arc.TailDirection);
			l_ArcObj->SetNumberField("tmu", l_Arc.TailControlPointLengthMultiplier);
			/// ??? No idea what that is but it's there
			l_ArcObj->SetNumberField("m", 0);
			l_Sliders.Add(l_ArcObj);
		}

		l_JsonObject->SetObjectArrayField("colorNotes", l_Notes);
		l_JsonObject->SetObjectArrayField("events", TArray<UVaRestJsonObject*>());
		l_JsonObject->SetObjectArrayField("obstacles", TArray<UVaRestJsonObject*>());

		return l_JsonObject;
	}*/



	return SerializeToObject();
}

bool FMapDifficulty::HasRequirement(EDefaultRequirement requirement)
{
	FString l_Requirement = Structures::DefaultRequirementToString(requirement);
	return HasRequirement(l_Requirement);
}

bool FMapDifficulty::HasRequirement(FString requirement)
{
	if (!DifficultyCustomData) return false;

	return DifficultyCustomData->Requirements.Contains(requirement);
}

FWallData::FWallData() : Height(0), Width(0)
{
	MARK_JSON_FLOAT_ALIASES(Beat, "b");
	MARK_JSON_FLOAT_ALIASES(Beat, "_time");

	MARK_JSON_FLOAT_ALIASES(Line, "x");
	MARK_JSON_FLOAT_ALIASES(Line, "_lineIndex");

	MARK_JSON_FLOAT_ALIASES(Layer, "y");
	MARK_JSON_FLOAT_ALIASES(Layer, "_lineLayer");

	MARK_JSON_FLOAT_ALIASES(Width, "w");
	MARK_JSON_FLOAT_ALIASES(Width, "_width");

	MARK_JSON_FLOAT_ALIASES(Height, "h");

	MARK_JSON_FLOAT_ALIASES(Duration, "d");
	MARK_JSON_FLOAT_ALIASES(Duration, "_duration");

	MARK_JSON_FLOAT_ALIASES(Type, "_type");

	MARK_SERIALIZE_IGNORED("_type");
}

void FWallData::PostParse()
{
	if (Height == 0) {
		Height = Type == 0 ? 4 : 5;
	}

	if (Type != 0) {
		Layer = 1;
	}
}

FArcData::FArcData()
{
	MARK_JSON_FLOAT_ALIASES(Beat, "b");
	MARK_JSON_INT64_ALIASES(Color, "c");
	MARK_JSON_INT64_ALIASES(Line, "x");
	MARK_JSON_INT64_ALIASES(Layer, "y");
	MARK_JSON_INT64_ALIASES(HeadDirection, "d");
	MARK_JSON_FLOAT_ALIASES(HeadControlPointLengthMultiplier, "mu");
	MARK_JSON_FLOAT_ALIASES(TailBeat, "tb");
	MARK_JSON_FLOAT_ALIASES(TailLine, "tx");
	MARK_JSON_FLOAT_ALIASES(TailLayer, "ty");
	MARK_JSON_FLOAT_ALIASES(TailDirection, "tc");
	MARK_JSON_FLOAT_ALIASES(TailControlPointLengthMultiplier, "tmu");
	MARK_JSON_FLOAT_ALIASES(m, "m");
}

FDifficultyBeatmapSet::FDifficultyBeatmapSet()
{
	MARK_JSON_STRING_ALIASES(Name, "_beatmapCharacteristicName");
	MARK_JSON_ARRAY_ALIASES(DifficultyBeatmaps, "_difficultyBeatmaps");
}

FMapDifficultyCustomData::FMapDifficultyCustomData() : Requirements(), Warnings(), Informations(), Suggestions()
{
	MARK_JSON_STRING_ALIASES(DifficultyLabel, "_difficultyLabel");
	
}
