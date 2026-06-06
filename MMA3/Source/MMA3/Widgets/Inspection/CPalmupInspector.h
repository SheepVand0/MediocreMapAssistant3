// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MMA3/Core/Structures.h"
#include "MMA3/Widgets/Components/CustomTextBlock.h"
#include "CPalmupInspector.generated.h"

/**
 * 
 */
UCLASS()
class MMA3_API UCPalmupInspector : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(BlueprintReadOnly) bool AnalysisStarted = false;
	float LeftAngle = 0;
	float RightAngle = 0;
	bool LeftUp = false; 
	bool RightUp = false;
	FNoteData* LastLeftNote = nullptr;
	FNoteData* LastRightNote = nullptr;
	float LastLeftAngle = 0;
	float LastRightAngle = 0;
	
	virtual void NativeConstruct() override;
	
	void EventNotePassed(FNoteData* noteData);
	
	UFUNCTION() void StartAnalysis();
	
	void UpdateText();
	
public:
	
	UPROPERTY(meta = (BindWidget)) class UCustomTextBlock* DegreesText;
	
	UPROPERTY(meta = (BindWidget)) class UCustomButton* MarkAnalysisBeginningButton;
	
	UPROPERTY(meta = (BindWidget)) class UImage* LeftDirectionImage;
	
	UPROPERTY(meta = (BindWidget)) class UImage* RightDirectionImage;
	
	
};
