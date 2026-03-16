// Fill out your copyright notice in the Description page of Project Settings.


#include "CHistory.h"
#include "MMA3/Core/Controller/CController.h"
#include "MMA3/Core/CObjectsSpawner.h"

FHistoryAction::FHistoryAction()
{

}

FHistory::FHistory() : History()
{

}

void FHistory::Undo() {

	if (History.IsEmpty()) return;

	History.Last(0)->Reverse();
	History.RemoveAt(History.Num() - 1);
}

void FHistory::Remember(FHistoryAction* action)
{
	History.Add(action);
}

void FNoteAction::Reverse()
{
	int x = GetMappingController()->MapContent->_notes.Remove(Note);
	
	GetObjectsSpawner()->SpawnObjects(GetMappingController()->MapContent, GetMappingController()->GetBeatCell(), 0.0f, 0.0f);
	GetMappingController()->UpdateBeatGrid();

	UE_LOG(LogTemp, Display, TEXT("Undoed: %d"), x);
}

void FWallAction::Reverse()
{
	GetMappingController()->MapContent->_walls.Remove(Wall);

	GetObjectsSpawner()->SpawnObjects(GetMappingController()->MapContent, GetMappingController()->GetBeatCell(), 0.0f, 0.0f);
	GetMappingController()->UpdateBeatGrid();
}

void FNoteDeleteAction::Reverse()
{
	GetMappingController()->MapContent->_notes.Add(Note);
	//GetMappingController()->MapContent->_notes.Sort();
	GetMappingController()->UpdateBeatGrid();
}

void FWallDeleteAction::Reverse()
{
	GetMappingController()->MapContent->_walls.Add(Wall);
	//GetMappingController()->MapContent->_walls.Sort();
	GetMappingController()->UpdateBeatGrid();
};