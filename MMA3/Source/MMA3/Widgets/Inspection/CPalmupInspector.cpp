// Fill out your copyright notice in the Description page of Project Settings.


#include "CPalmupInspector.h"

#include "MMA3/Core/BeatmapObjects/CNote.h"
#include "MMA3/Core/Controller/CController.h"

void UCPalmupInspector::NativeConstruct()
{
	Super::NativeConstruct();

	GetMappingController()->UniqueNotePassedCallback = new SMethodPtr(this, &UCPalmupInspector::EventNotePassed);

	MarkAnalysisBeginningButton->OnClicked.AddDynamic(this, &UCPalmupInspector::StartAnalysis);
}

void UCPalmupInspector::EventNotePassed(FNoteData* noteData)
{
	if (!AnalysisStarted) return;

	float* usedAngle = noteData->Type == 0 ? &LeftAngle : &RightAngle;
	bool* usedUp = noteData->Type == 0 ? &LeftUp : &RightUp;
	FNoteData** lastNoteDataHolder = noteData->Type == 0 ? &LastLeftNote : &LastRightNote;
	float* lastAngle = noteData->Type == 0 ? &LastRightAngle : &LastLeftAngle;
	
	int* temp = (ACNote::RotationByCutDirection.Find(noteData->Direction));
	int rotation;
	
	
	FNoteData* lastNoteData = *lastNoteDataHolder;
	if (lastNoteData)
	{
		if (abs(lastNoteData->Beat - noteData->Beat) < 0.05f) return;
	}
	
	if (!temp)
	{
		
		FVector2f direction = FVector2f(noteData->Line - lastNoteData->Line,  noteData->Layer - lastNoteData->Layer);
		if (direction.Normalize())
		{
			float angle = direction.Y > 0 ? (acosf(direction.X) / PI * 180) : -(acosf(direction.X) / PI * 180);
			if (noteData->Type == 0)
			{
				angle = -(360.0f - angle);	
			}
			
			angle -= 90.0f;
			if (abs(angle - *lastAngle) < 90)
			{
				angle += 180;
			}
			
			UE_LOG(LogTemp, Display, TEXT("Found Angle: %f"), angle);
			
			angle = static_cast<int>(angle / 45.f) * 45;
			
			rotation = angle;
		} else
		{
			rotation = *lastAngle + 180;
		}
	} else
	{
		rotation = *temp;
	}

	if (*usedUp)
	{
		if (noteData->Type == 0)
		{
			rotation += 180;
			rotation = (rotation + 360) % 360;
		}
		else
		{
			rotation += 180;
			rotation = (rotation + 360) % 360;
		}
	}
	
	*usedUp = !(*usedUp);

	float amount = *usedAngle - rotation;
	*usedAngle -= amount;
	
	*lastNoteDataHolder = noteData;
	*lastAngle = rotation;
	
	UpdateText();
}

void UCPalmupInspector::StartAnalysis()
{
	LeftAngle = 0.f;
	RightAngle = 0.f;
	LeftUp = false;
	RightUp = false;
	AnalysisStarted = true;
}

void UCPalmupInspector::UpdateText()
{
	FString text = FString::SanitizeFloat(LeftAngle, 1) + " - " + FString::SanitizeFloat(
		(-(int)RightAngle + 360) % 360, 1);
	DegreesText->SetText(FText::FromString(text));

	LeftDirectionImage->SetRenderTransformAngle(180 - LeftAngle);
	RightDirectionImage->SetRenderTransformAngle(180 - RightAngle);
}
