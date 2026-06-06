// Fill out your copyright notice in the Description page of Project Settings.


#include "CWJumpEditor.h"
#include "MMA3/Core/Controller/CController.h"

FPattern::FPattern(TArray<FNoteData*> notes)
{
	Notes = notes;
}

FPattern UCWJumpEditor::CreateJump(int color, float startBeat, float endBeat, float div)
{
	int l_ItCount = (endBeat - startBeat) * div;

	TArray<FNoteData*> l_Notes;

	int l_Index = -1;

	TArray<int> l_FrontNotes;

	int l_Second = -1;

	TArray<int> l_BackNotes;

	for (int x = 0; x < l_ItCount; x++) {
		float l_Beat = startBeat + (x * (1.f / div));

		int l_Mod = x % 2;

		if (!l_Mod) {
			TArray<int> l_temp = GetMappingController()->GetAllNotesIndexInSectionExclusive(l_Beat, 1.f / div, l_FrontNotes.IsEmpty() ? 0 : l_FrontNotes.Last(0));

			if (!l_temp.IsEmpty()) {
				l_FrontNotes = l_temp;
				continue;
			}
		}
		else {
			TArray<int> l_temp = GetMappingController()->GetAllNotesIndexInSectionExclusive(l_Beat, 1.f / div, l_BackNotes.IsEmpty() ? 0 : l_BackNotes.Last(0));

			if (!l_temp.IsEmpty()) {
				l_BackNotes = l_temp;
				continue;
			}
		}

		TArray<int>* l_usedArray = &(l_Mod ? l_BackNotes : l_FrontNotes);

		for (int i = 0; i < l_usedArray->Num(); i++) {
			FNoteData* l_Note = GetMappingController()->MapContent->_notes[(*l_usedArray)[i]];

			FNoteData* l_New = new FNoteData();
			l_New->Beat = (l_Beat + FloatMod(l_Note->Beat, 1.f / div));
			l_New->Line = l_Note->Line;
			l_New->Layer = l_Note->Layer;
			l_New->Direction = l_Note->Direction;
			l_New->Type = l_Note->Type;
			l_Notes.Add(l_New);
		}
	}

	FPattern* l_Pattern = new FPattern(l_Notes);
	CurrentPattern = l_Pattern;
	return *l_Pattern;
}

TArray<FNoteData*> UCWJumpEditor::LerpJumpUnique(float beat, float div, int startIndex, bool allowVisionBlock, float& outEndBeat, int& outLastIndex)
{
	//UE_LOG(LogTemp, Display, TEXT("FOUND BEAT: %f"), beat);
	
	TArray<FNoteData*> l_Preview;

	auto l_MapContent = GetMappingController()->MapContent;
	
	TArray<int> l_Notes = GetMappingController()->GetAllNotesIndexInSectionInclusive(beat, (1.f/div), startIndex);
	
	for (int x = 0; x < l_Notes.Num();x++)
	{
		UE_LOG(LogTemp, Display, TEXT("INDEX: %d, value: %f"), x, l_MapContent->_notes[l_Notes[x]]->Beat);
	}
		
	if (l_Notes.Num() == 0) return l_Preview;

	if (!l_Notes.IsEmpty()) {
		outLastIndex = l_Notes.Last();
	}

	float l_EndBeat = GetMappingController()->MapContent->_notes[FMath::Clamp(l_Notes.Last() + 1, 0, l_MapContent->_notes.Num() - 1)]->Beat;
	outEndBeat = l_EndBeat;
	if (outEndBeat == beat) return l_Preview;

	float l_ItCount = ((l_EndBeat - beat) * div) - 1;

	UE_LOG(LogTemp, Display, TEXT("Notes pattern count: %d"), l_Notes.Num());
	//UE_LOG(LogTemp, Display, TEXT("BEAT: %f"), beat);

	for (int x = 0; x <= l_ItCount; x++) {
		for (int n = 0; n < l_Notes.Num(); n++) {
			float l_Beat = beat + (1.f / div) + (static_cast<float>(x) * (1.f / div));


			FNoteData* l_Note = l_MapContent->_notes[l_Notes[n]];
			FNoteData* l_Next = l_MapContent->_notes[FMath::Clamp(l_Notes.Last() + 1 + n, 0, l_MapContent->_notes.Num() - 1)];
			UE_LOG(LogTemp, Display, TEXT("Next note index: %d"), FMath::Clamp(l_Notes.Last() + 1 + n, 0, l_MapContent->_notes.Num() - 1));

			FNoteData* l_New = new FNoteData();
			l_New->Beat = l_Beat + (l_Note->Beat - beat);
			UE_LOG(LogTemp, Display, TEXT("Iteration: %d, beat calculated: %f"), x, l_New->Beat);
			
			float l_Percentage = ApplyMethod(static_cast<float>(x) / (l_ItCount), PercentageMethod);
			
			float l_PercRounded = l_Percentage;
			while (l_PercRounded >= 0.25f)
			{
				l_PercRounded -= 0.25f;
			}
			
			if (l_PercRounded >= 0.25f / 2.f)
			{
				l_PercRounded = FMath::Clamp(l_Percentage - l_PercRounded + 0.25f, 0, 1);
			} else
			{
				l_PercRounded = l_Percentage - l_PercRounded;
			}
			
			l_New->Line = l_Note->Line + static_cast<int>((l_Next->Line - l_Note->Line) * l_PercRounded);
			l_New->Layer = l_Note->Layer + static_cast<int>((l_Next->Layer - l_Note->Layer) * l_PercRounded);
			
			if (!allowVisionBlock)
			{
				if (l_New->Layer == 1 && l_New->Line >= 1 && l_New->Line <= 2)
				{
					float l_XDist = FMath::Abs(l_New->Line - l_Note->Line);
					float l_YDist = FMath::Abs(l_New->Layer - l_Note->Layer);
					
					float l_PercentageRelative = l_Percentage - 0.5f;
					if (!l_PercentageRelative)
					{
						l_PercentageRelative = 1.f;
					}
					
					if (l_XDist < l_YDist)
					{
						/*do
						{
							l_New->Line += l_PercentageRelative/(FMath::Abs(l_PercentageRelative));
						} while (l_New->Line >= 1 && l_New->Line <= 2);*/
						
						if (l_New->Line == 1)
						{
							l_New->Line -= 1;
						} else
						{
							l_New->Line += 1;
						}
					} else
					{
						l_New->Layer -= l_PercentageRelative/(FMath::Abs(l_PercentageRelative));
					}
				}
			}
			
			l_New->Type = l_Note->Type;

			float l_Angle = *ACNote::RotationByCutDirection.Find(l_Note->Direction);
			float l_NextAngle = *ACNote::RotationByCutDirection.Find(l_Next->Direction);
			float l_NewDir = 0;
			
			float l_Sub = l_NextAngle - l_Angle;
			//UE_LOG(LogTemp, Display, TEXT("SUB ANGLE: %f"), l_Sub);
			
			if (l_Sub >= -180 && l_Sub <= 180)
				l_NewDir = l_Angle + ((l_NextAngle - l_Angle) * l_Percentage);
			else
				l_NewDir = l_Angle - ((360 - (l_NextAngle - l_Angle)) * l_Percentage);
				
			int l_FixedNewDire = ((static_cast<int>(l_NewDir / 45) * 45) + 360) % 360;

			l_New->Direction = ACNote::CutDirectionFromAngle(l_FixedNewDire);

			l_Preview.Add(l_New);
		}
	}

	return l_Preview;
}

float UCWJumpEditor::ApplyMethod(float percentage, EPercentageMethod method)
{
	switch (method)
	{
	default: return percentage;
	case EPercentageMethod::Pow:
		return FMath::Pow(percentage, ExponentSpinBox->GetValue());
		break;
	case EPercentageMethod::Sin:
		return (FMath::Sin((percentage * ExponentSpinBox->GetValue() * PI_ON_TWO) - PI_ON_TWO) + 1) / 2.f;
		break;
	}
}

EPercentageMethod UCWJumpEditor::StringToPercentageMethod(const FString& value)
{
	if (value == "Pow") return EPercentageMethod::Pow;
	if (value == "Sinus") return EPercentageMethod::Sin; 
	return EPercentageMethod::Linear;
}


FPattern UCWJumpEditor::CreateJumpWithLerp(int color, float startBeat, float endBeat, float div)
{
	ClearCurrentPattern();

	float l_OutBeat = startBeat;
	int l_Index = 0;
	TArray<FNoteData*> l_Final;
	
	
	while (l_OutBeat < endBeat) {
		TArray<FNoteData*> l_Result = LerpJumpUnique(l_OutBeat, div, l_Index, false, l_OutBeat, l_Index);

		/*if (l_Result.IsEmpty()) {
			return FPattern({});
		}*/

		for (int x = 0; x < l_Result.Num(); x++) {
			if (l_Result[x]->Beat <= endBeat) {
				l_Final.Add(l_Result[x]);
			}
			else {
				break;
			}
		}

		/*if (l_OutBeat == startBeat) 
			return FPattern({});*/
	}

	//ClearCurrentPattern();
	CurrentPattern = new FPattern(l_Final);
	//UE_LOG(LogTemp, Display, TEXT("SIIIIZE: %d"), CurrentPattern->Notes.Num());
	return l_Final;
}

void UCWJumpEditor::ClearCurrentPattern()
{
	if (CurrentPattern) {
		for (auto l_Note : DisplayedNotes) {
			if (l_Note) {
				l_Note->Destroy();
			}
		}

		DisplayedNotes.Empty();

		/*for (auto x = 0; x < CurrentPattern->Notes.Num(); x++) {
			if (CurrentPattern->Notes[x])
			{
				free(CurrentPattern->Notes[x]);
			}
		}*/

		//free(CurrentPattern);
		CurrentPattern = nullptr;
	}
}

void UCWJumpEditor::UpdateNotesPreview()
{
	if (!CurrentPattern) return;

	for (auto l_Note : DisplayedNotes) {
		if (l_Note) {
			l_Note->Destroy();
		}
	}

	for (auto x : CurrentPattern->Notes) {
		ACNote* l_Note = GetObjectsSpawner()->SpawnObject<ACNote>(x, GetMappingController()->GetBeatCell());
		l_Note->SetPreview();
		UE_LOG(LogTemp, Display, TEXT("[UpdateNotesPreview] Beat : %f"), x->Beat);
		DisplayedNotes.Add(l_Note);
	}
}

void UCWJumpEditor::CreateJumpFromSelection(int color, float div)
{
	CreateJump(color, GetMappingController()->SelectionFirstBPM, GetMappingController()->SelectionEndBPM, div);

	UpdateNotesPreview();
}

void UCWJumpEditor::CreateLerpJumpFromSelection(float div)
{
	CreateJumpWithLerp(0, GetMappingController()->SelectionFirstBPM, GetMappingController()->SelectionEndBPM, div);

	UpdateNotesPreview();
}

void UCWJumpEditor::ApplyPreviewOnMap()
{
	if (DisplayedNotes.IsEmpty()) return;

	for (auto x : DisplayedNotes) {
		GetMappingController()->MapContent->_notes.Add(x->NoteData);

		x->Destroy();
	}

	GetMappingController()->SortMapContent();

	GetMappingController()->ResetCurrent();

	DisplayedNotes.Empty();
	CurrentPattern = nullptr;
}

void UCWJumpEditor::ShuffleSelected(int line, int layer, int maxSpace, bool allowVisionBlock, bool shuffleLine, bool shuffleLayer)
{
	auto l_Array = GetMappingController()->GetSelectedNotes();

	for (auto x : l_Array) {
		if (shuffleLayer) {
			x->Layer = layer != -1 ? FMath::RandRange(0, 2) : x->Layer;

			if (!allowVisionBlock) {
				if (x->Layer == 1) {
					x->Layer += FMath::RandRange(0, 1) == 1 ? 1 : -1;
				}
			}
		}

		if (shuffleLine) {
			x->Line = line != -1 ? FMath::RandRange(0, 3) : x->Line;
		}
	}

	GetMappingController()->ResetCurrent();
}

float UCWJumpEditor::FloatMod(float value, float mod)
{
	float l_asFloat = value;

	while (l_asFloat >= mod) {
		l_asFloat -= mod;
	}

	return l_asFloat;
}
