// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "BeatmapUtils.h"

// Sets default values
AC_Controller::AC_Controller()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>l_Cube(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>l_Bomb(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/Bomb.Bomb'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>l_Dot(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/NoteBody_Cylinder_001.NoteBody_Cylinder_001'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>l_MappingGridMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/MappingGridPlane.MappingGridPlane'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance>l_CubeMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/Mapping/M_BasicWhiteShape.M_BasicWhiteShape'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance>l_BombMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/Mapping/M_Bomb.M_Bomb'"));
	ConstructorHelpers::FObjectFinder<USoundWave> l_HitSound(TEXT("/Script/Engine.SoundWave'/Game/Assets/Sounds/HitSounds/HitSoundb.HitSoundb'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_WallMaterial(TEXT("/Script/Engine.Material'/Game/Assets/Materials/M_Obstacle.M_Obstacle'"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));

	PlayingTime = 0;
	ActorTime = 0;

	HitSound = l_HitSound.Object;

	TimeMarkerCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	TimeMarkerCube->SetStaticMesh(l_Cube.Object);
	TimeMarkerCube->SetRelativeScale3D(FVector(1, 0.05f, 0.05f));
	TimeMarkerCube->SetMaterial(0, l_CubeMat.Object);

	Instance = this;

	BombMesh = l_Bomb.Object;
	DotMesh = l_Dot.Object;

	MappingGrid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mapping Grid"));
	MappingGrid->SetStaticMesh(l_MappingGridMesh.Object);

	WallMaterial = l_WallMaterial.Object;
}

// Called when the game starts or when spawned
void AC_Controller::BeginPlay()
{
	Super::BeginPlay();

	if (!UGameplayStatics::DoesSaveGameExist(MMA_SAVE_GAME_SLOT_NAME, 0)) {
		UGameplayStatics::SaveGameToSlot(UGameplayStatics::CreateSaveGameObject(UMMAConfig::StaticClass()), MMA_SAVE_GAME_SLOT_NAME, 0);
	}
	UMMAConfig::Instance = Cast<UMMAConfig>(UGameplayStatics::LoadGameFromSlot(MMA_SAVE_GAME_SLOT_NAME, 0));
}

void AC_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->ActorTime = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());

	if (PlayerControllerReference == nullptr)
		PlayerControllerReference = GetWorld()->GetFirstPlayerController();

	if (CurrentScene == "MainMenu") {
		PlayerControllerReference->bShowMouseCursor = true;
		return;
	}

	if (Playing) {
		PlayingTime += (ActorTime - StartedPlayTime - PlayingTime);
		UpdateBeatGrid();
		OnTimeUpdated.Broadcast(PlayingTime);
	}
}

void AC_Controller::GenerateGrid(FMapInfo p_Info, FString p_Diff, FString p_Mode) {
	if (p_Info.Song == nullptr) { UE_LOG(LogTemp, Error, TEXT("Song is not valid")); return; }

	MapData = p_Info;

	FString l_MapData;
	FFileHelper::LoadFileToString(l_MapData, *FString(p_Info.MapPath + "\\" + p_Diff + p_Mode + ".dat"));
	MapContent.FromJson(l_MapData);

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

	BeatCells = l_World->SpawnActor<ABeatCell>();
	BeatCells->SetActorLocation(GetActorLocation());
	BeatCells->SetLength(l_RoundedBeatCount);


	////////////////////////////////////////////////////////////////////////////////////////////
	// Note spawning

	OnNeedToAddBeatmapObjects.Broadcast();
}

void AC_Controller::Play() {
	if (MapData.Song == nullptr) return;
	if (PlayingTime >= MapData.Song->Duration) return;
	Playing = true;
	if (PlayingTime < 0) PlayingTime = 0;
	StartedPlayTime = ActorTime - PlayingTime;
	AudioComponent->Sound = MapData.Song;
	AudioComponent->Play(PlayingTime);
}

void AC_Controller::Stop() {
	if (!Playing) return;
	Playing = false;
	AudioComponent->Stop();
}

void AC_Controller::AddTime(float p_Value) {
	if (BeatCells == nullptr) return;
	if (Playing) return;
	if (PlayingTime + p_Value < 0) {
		PlayingTime = 0;
		UpdateBeatGrid();
		return;
	}

	if (PlayingTime + p_Value > MapData.Song->GetDuration()) {
		PlayingTime = MapData.Song->GetDuration();
		UpdateBeatGrid();
		return;
	}

	PlayingTime += p_Value;
	UpdateBeatGrid();
}

void AC_Controller::UpdateBeatGrid() {
	auto l_RootPos = BeatCells->GetActorLocation();
	BeatCells->SetActorLocation(FVector(l_RootPos.X, (PlayingTime * -100) * (MapData.BPM / 60), l_RootPos.Z));
}