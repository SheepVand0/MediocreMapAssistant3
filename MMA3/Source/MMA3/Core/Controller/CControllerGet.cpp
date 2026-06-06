#include "CController.h"
#include "MMA3/Core/CObjectsSpawner.h"

float ACController::GetBeat()
{
	return (MapInfo->_beatsPerMinute / 60) * PlayingTime;
}

float ACController::GetDeltaBeat()
{
	return (MapInfo->_beatsPerMinute / 60) * DeltaTime;
}

float ACController::GetPlayTime()
{
	return PlayingTime;	
}

bool ACController::IsPlaying()
{
	return Playing;
}

FMapData* ACController::GetMapData()
{
	return MapContent;
}

FMapInfo* ACController::GetMapInfo()
{
	return MapInfo;
}

FMapDifficulty* ACController::GetMapDifficulty()
{
	return Difficulty;
}

FString ACController::GetCurrentSceneName()
{
	return CurrentScene;
}

ABeatCell* ACController::GetBeatCell()
{
	return BeatCells;
}

float ACController::BeatToTime(float beat)
{
	return (60.f / MapInfo->_beatsPerMinute) * beat;
}

int ACController::GetNoteIndexByBeat(float beat, float maxError, int baseIndex)
{
	if (!MapContent) return -1;

	for (int x = FMath::Clamp(baseIndex, 0, MapContent->_notes.Num()); x < MapContent->_notes.Num();x++) {
		auto note = MapContent->_notes[x];
		if (note->Beat >= beat) {
			
			if (note->Beat <= beat + maxError) {
				return x;
			}
			else {
				return -1;
			}
		}
	}

	return -1;
}

TArray<int> ACController::GetAllNotesIndexInSectionExclusive(float beat, float length, int baseIndex)
{
	TArray<int> l_Result;

	for (int x = FMath::Clamp(baseIndex, 0, MapContent->_notes.Num()); x < MapContent->_notes.Num(); x++) {
		auto note = MapContent->_notes[x];
		if (note->Beat >= beat && note->Beat < beat + length) {
			l_Result.Add(x);
		}

		if (note->Beat > beat + length) break;
	}

	return l_Result	;
}

TArray<int> ACController::GetAllNotesIndexInSectionInclusive(float beat, float length, int baseIndex)
{
	TArray<int> l_Result;

	for (int x = FMath::Clamp(baseIndex, 0, MapContent->_notes.Num()); x < MapContent->_notes.Num(); x++) {
		auto note = MapContent->_notes[x];
		if (note->Beat >= beat && note->Beat <= beat + length) {
			l_Result.Add(x);
		}

		if (note->Beat > beat + length) break;
	}
	
	l_Result.Sort();
	
	return l_Result;
}

TArray<FNoteData*> ACController::GetSelectedNotes()
{
	return SelectedNotes;
}

FNoteData* ACController::GetLastSameColorNote(FNoteData* current) const
{
	if (!MapContent) return nullptr;

	int l_LastIndex = GetObjectsSpawner()->GetLastDisplayedNoteIndex();

	for (int x = l_LastIndex; x > 0; x--) {
		auto l_Note = MapContent->_notes[x];
		if (l_Note->Type == current->Type && l_Note != current && l_Note->Beat < current->Beat) {
			return l_Note;
		}
	}

	return nullptr;
}

FNoteData* ACController::GetLastSameColorNote(float beat, int type)
{
	if (!MapContent) return nullptr;

	int l_LastIndex = GetObjectsSpawner()->GetLastDisplayedNoteIndex();

	for (int x = l_LastIndex; x > 0; x--) {
		auto l_Note = MapContent->_notes[x];
		if (l_Note->Type == type && l_Note->Beat < beat) {
			return l_Note;
		}
	}

	return nullptr;
}

