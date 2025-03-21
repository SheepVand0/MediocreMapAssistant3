// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ObjectsSpawner.h"

// Sets default values
AC_ObjectsSpawner::AC_ObjectsSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AC_ObjectsSpawner::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle l_Handle;

	AC_Controller::Instance->OnNeedToAddBeatmapObjects.AddDynamic(this, &AC_ObjectsSpawner::SpawnObjects);
}

// Called every frame
void AC_ObjectsSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_ObjectsSpawner::SpawnObjects(FMapData mapContent, ABeatCell* beatCell) {
	UE_LOG(LogTemp, Warning, TEXT("Note count : %d"), mapContent.Notes.Num());

	TArray <AActor*> l_Notes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AC_Note::StaticClass(), l_Notes);
	for (int l_i = 0; l_i < l_Notes.Num(); l_i++) {
		l_Notes[l_i]->Destroy();
	}

	TArray<AActor*> l_Walls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AC_Wall::StaticClass(), l_Walls);
	for (int l_i = 0; l_i < l_Walls.Num(); l_i++) {
		l_Walls[l_i]->Destroy();
	}

	ABeatCell* l_BeatCell = beatCell;

	for (int l_i = 0; l_i < mapContent.Notes.Num(); l_i++) {
		auto l_Note = Cast<AC_Note>(GetWorld()->SpawnActor(AC_Note::StaticClass()));
		FNoteData l_Current = mapContent.Notes[l_i];
		l_Note->SetNoteData(l_Current.Beat, l_Current.Type, l_Current.Line, l_Current.Layer, l_Current.Direction);
		l_Note->AttachToActor(l_BeatCell, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
	}

	for (int l_i = 0; l_i < mapContent.Walls.Num(); l_i++) {
		auto l_Wall = Cast<AC_Wall>(GetWorld()->SpawnActor(AC_Wall::StaticClass()));
		FWallData l_Current = mapContent.Walls[l_i];
		l_Wall->SetData(l_Current.Beat, l_Current.Type, l_Current.Line, l_Current.Width, l_Current.Duration);
		l_Wall->AttachToActor(l_BeatCell, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
	}
}