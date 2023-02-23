// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "BeatCell.h"
#include "BeatmapUtils.h"

// Sets default values
AC_Controller::AC_Controller()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BeatCellsRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Beat Cells Root"));

}

// Called when the game starts or when spawned
void AC_Controller::BeginPlay()
{
	Super::BeginPlay();

	/*try {
		m_MapSelectionWidget = CreateWidget<ULevelSelectionWidget>(GetWorld()->GetFirstPlayerController(), m_MapSelectionWidgetModel);
	}
	catch(std::exception) {
		GEngine->AddOnScreenDebugMessage(0, 10, FColor::Red, FString("Error happened"));
	}*/

}

// Called every frame
void AC_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerControllerReference == nullptr)
		PlayerControllerReference = GetWorld()->GetFirstPlayerController();

	if (CurrentScene == "MainMenu")
		PlayerControllerReference->bShowMouseCursor = true;
}

void AC_Controller::GenerateGrid(FMapInfo p_Info) {
	if (p_Info.Song == nullptr) { UE_LOG(LogTemp, Error, TEXT("Song is not valid")); return; }

	TArray < AActor*, FDefaultAllocator> l_Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABeatCell::StaticClass(), l_Actors);
	for (int l_i = 0; l_i < l_Actors.Num(); l_i++)
	{
		l_Actors[l_i]->Destroy();
	}

	float l_OriginalBeatCount = BeatmapUtils::CalculateBeatCount(p_Info.Song->Duration, p_Info.BPM);
	int l_RoundedBeatCount = FMath::Floor(l_OriginalBeatCount);
	UE_LOG(LogTemp, Display, TEXT("Is BeatCount null ? %s"), (l_RoundedBeatCount == NULL ? TEXT("True") : TEXT("False")));
	UWorld* l_World = GetWorld();
	//if (l_RoundedBeatCount > 16) { l_RoundedBeatCount = 16; }
	//GEngine->AddOnScreenDebugMessage(4, 10.0f, FColor::White, FString::SanitizeFloat(l_RoundedBeatCount));
	/*FActorSpawnParameters l_Parameters = FActorSpawnParameters{};
	l_Parameters.bNoFail = true;*/

	ABeatCell* l_Cell = l_World->SpawnActor<ABeatCell>();
	l_Cell->SetActorLocation(GetActorLocation());
	l_Cell->AttachToComponent(BeatCellsRoot, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false));
	l_Cell->SetLength(l_RoundedBeatCount);
}
