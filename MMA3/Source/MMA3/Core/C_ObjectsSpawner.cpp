// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ObjectsSpawner.h"

// Sets default values
AC_ObjectsSpawner::AC_ObjectsSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BindedEvent = false;
}

// Called when the game starts or when spawned
void AC_ObjectsSpawner::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle l_Handle;

	//GetWorld()->GetTimerManager().SetTimer(l_Handle, [this]() {

	//}, 0, false, 0.2f);
}

// Called every frame
void AC_ObjectsSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!BindedEvent) {
		if (AC_Controller::Instance == nullptr) return;

		AC_Controller::Instance->OnNeedToAddBeatmapObjects.AddDynamic(this, &AC_ObjectsSpawner::SpawnObjects);
		BindedEvent = true;
	}
}

void AC_ObjectsSpawner::SpawnObjects() {
	AC_Controller* l_Controller = AC_Controller::Instance;
	TArray <AActor*> l_Notes;
	UGameplayStatics::GetAllActorsOfClass(l_Controller->GetWorld(), AC_Note::StaticClass(), l_Notes);
	for (int l_i = 0; l_i < l_Notes.Num(); l_i++) {
		l_Notes[l_i]->Destroy();
	}

	for (int l_i = 0; l_i < l_Controller->MapContent.Notes.Num(); l_i++) {
		auto l_Note = Cast<AC_Note>(l_Controller->GetWorld()->SpawnActor(AC_Note::StaticClass()));
		FNoteData l_Current = l_Controller->MapContent.Notes[l_i];
		l_Note->SetNoteData(l_Current.Beat, l_Current.Type, l_Current.Line, l_Current.Layer, l_Current.Direction);
	}
}