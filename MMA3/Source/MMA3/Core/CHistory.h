// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMA3/Core/Structures.h"


enum class EActionType : uint8 {
	NotePlaced
};

struct FHistoryAction {

public:

	FHistoryAction();

	virtual void Reverse() {
		UE_LOG(LogTemp, Display, TEXT("Did the emptyy one"));
	};
};

struct FHistory {

public:

	FHistory();

	TArray<FHistoryAction*> History;

	void Remember(FHistoryAction* action);

	void Undo();

};

struct FNoteAction : public FHistoryAction {

public:

	FNoteData* Note;

	FNoteAction(FNoteData* noteData) : FHistoryAction() {
		Note = noteData;
	}

	virtual void Reverse() override;
};


struct FWallAction : public FHistoryAction {

public:

	FWallData* Wall;

	FWallAction(FWallData* wall) : FHistoryAction() {
		Wall = wall;
	}
 
	virtual void Reverse() override;

};

struct FNoteDeleteAction : public FHistoryAction {

public:

	FNoteData* Note;

	FNoteDeleteAction(FNoteData* noteData) : FHistoryAction() {
		Note = noteData;
	}

	virtual void Reverse() override;

};

struct FWallDeleteAction : public FHistoryAction {

public:

	FWallData* Wall;

	FWallDeleteAction(FWallData* wall) : FHistoryAction() {
		Wall = wall;
	}

	virtual void Reverse() override;

};