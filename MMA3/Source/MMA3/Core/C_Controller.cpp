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
	ConstructorHelpers::FObjectFinder<UMaterial> l_WallMaterial(TEXT("/Script/Engine.Material'/Game/Assets/Materials/Mapping/M_Obstacle.M_Obstacle'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance>l_NoteMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/Mapping/M_NoteInstance.M_NoteInstance'"));
	ConstructorHelpers::FObjectFinder<UMaterial>l_WaveformMaterial(TEXT("/Script/Engine.Material'/Game/Assets/Materials/Waveform/M_SimpleMat.M_SimpleMat'"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));

	PlayingTime = 0;
	ActorTime = 0;

	HitSound = l_HitSound.Object;

	TimeMarkerCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	TimeMarkerCube->SetStaticMesh(l_Cube.Object);
	TimeMarkerCube->SetRelativeScale3D(FVector(1, 0.05f, 0.05f));
	TimeMarkerCube->SetMaterial(0, l_CubeMat.Object);

	Instance = this;

	GlobalNoteMaterial = l_NoteMaterial.Object;
	BombMesh = l_Bomb.Object;
	DotMesh = l_Dot.Object;

	MappingGrid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mapping Grid"));
	MappingGrid->SetStaticMesh(l_MappingGridMesh.Object);
	MappingGrid->SetCollisionObjectType(ECC_WorldStatic);
	MappingGrid->ComponentTags.Add("MappingGrid");

	WaveformMaterial = l_WaveformMaterial.Object;

	WaveformMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Waveform Mesh"));
	WaveformMesh->SetupAttachment(TimeMarkerCube);
	WaveformMesh->SetMaterial(0, WaveformMaterial);

	WallMaterial = l_WallMaterial.Object;

	Instance = this;
}

int32 AC_Controller::OnGeneratePCMAudio(TArray<uint8>& outPCM, int32 samplesGenerated)
{

	return int32();
}

// Called when the game starts or when spawned
void AC_Controller::BeginPlay()
{
	Super::BeginPlay();

	if (!UGameplayStatics::DoesSaveGameExist(MMA_SAVE_GAME_SLOT_NAME, 0)) {
		UGameplayStatics::SaveGameToSlot(UGameplayStatics::CreateSaveGameObject(UMMAConfig::StaticClass()), MMA_SAVE_GAME_SLOT_NAME, 0);
	}
	UMMAConfig::Instance = Cast<UMMAConfig>(UGameplayStatics::LoadGameFromSlot(MMA_SAVE_GAME_SLOT_NAME, 0));

	UpdateNotesMaterial();
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
		if (PlayingTime >= MapInfo.Song->GetDuration()) {
			PlayingTime = MapInfo.Song->GetDuration();
			Stop();
		}

		UpdateBeatGrid();
		OnTimeUpdated.Broadcast(PlayingTime);

		URenderWaveform::RenderWaveform(MapInfo.Song, MapInfo.SongPCMData, MapInfo.NeededSamples, WaveformMesh, PlayingTime, 100);
		//UE_LOG(LogTemp, Warning, TEXT("Vertex count : %d, %f"), WaveformMesh->GetProcMeshSection(0)->ProcVertexBuffer.Num(), PlayingTime);
	}

	EditModeWidget->UpdateWidget(PlayingTime, Playing);
}

void AC_Controller::GenerateGrid(FMapInfo p_Info, FString p_Diff, FString p_Mode) {
	if (p_Info.Song == nullptr) { UE_LOG(LogTemp, Error, TEXT("Song is not valid")); return; }

	MapInfo = p_Info;

	FString l_MapData;
	FMapDifficulty l_Difficulty;
	for (int l_i = 0; l_i < MapInfo.DifficultyBeatmapSets.Num(); l_i++) {
		if (MapInfo.DifficultyBeatmapSets[l_i].Name == p_Mode) {
			for (int l_i1 = 0; l_i1 < MapInfo.DifficultyBeatmapSets[l_i].DifficultyBeatmaps.Num(); l_i1++) {
				auto l_Diff = MapInfo.DifficultyBeatmapSets[l_i].DifficultyBeatmaps[l_i1];
				if (l_Diff.Difficulty == p_Diff) {
					l_Difficulty = l_Diff;
				}
			}
			break;
		}
	}

	FFileHelper::LoadFileToString(l_MapData, *FString(p_Info.MapPath + "\\" + l_Difficulty.BeatmapFileName));
	MapContent.FromJson(l_MapData);
	SongDuration = p_Info.Song->Duration;
	//UE_LOG(LogTemp, Display, TEXT("Notes count: %f"), MapContent.Notes.Num());

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

	if (BeatCells == nullptr)
		BeatCells = l_World->SpawnActor<ABeatCell>();

	BeatCells->SetActorLocation(GetActorLocation());
	BeatCells->SetLength(l_RoundedBeatCount);

	URenderWaveform::GenerateSpectrogramMesh(WaveformMesh, 100, (MapInfo.BPM / 60) * 100);

	////////////////////////////////////////////////////////////////////////////////////////////
	// Note spawning

	OnNeedToAddBeatmapObjects.Broadcast(MapContent, BeatCells);

	OnNeedToResetMapperPawnTransform.Broadcast();

	//////////////////////////////////////////////////////////////////////////////////////////
	/// Widget creation

	OnNeedToCreateEditModeWidget.Broadcast();

	EditModeWidget->OnTimeChanged.RemoveAll(this);
	EditModeWidget->OnTimeChanged.AddDynamic(this, &AC_Controller::WidgetTimeSliderChanged);
	EditModeWidget->Init(SongDuration);
	FInputModeGameAndUI l_InputMode;
	l_InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerControllerReference->SetInputMode(l_InputMode);

}

void AC_Controller::WidgetTimeSliderChanged(float newValue)
{
	SetTime(newValue);
}

void AC_Controller::Play() {
	if (MapInfo.Song == nullptr) return;
	if (PlayingTime >= MapInfo.Song->Duration) return;
	Playing = true;
	if (PlayingTime < 0) PlayingTime = 0;
	StartedPlayTime = ActorTime - PlayingTime;
	AudioComponent->Sound = MapInfo.Song;
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

	if (PlayingTime + p_Value > SongDuration && p_Value > 0) {
		PlayingTime = SongDuration;
		UpdateBeatGrid();
		return;
	}

	PlayingTime += p_Value;
	UpdateBeatGrid();
}

void AC_Controller::SetTime(float time)
{
	float l_Time = time;
	if (l_Time >= SongDuration)
		l_Time = SongDuration;

	if (l_Time < 0)
		l_Time = 0;

	PlayingTime = l_Time;
	UpdateBeatGrid();
	if (Playing) {
		AudioComponent->Stop();
		AudioComponent->Play(PlayingTime);
	}
}

void AC_Controller::UpdateBeatGrid() {
	float l_CurrentPosition = TimeMarkerCube->GetComponentLocation().Y;
	float l_YPosition = (PlayingTime * 100) * (MapInfo.BPM / 60);
	FVector l_NewPos = FVector(0, l_YPosition, 0);
	TimeMarkerCube->SetWorldLocation(l_NewPos);
	MappingGrid->SetWorldLocation(l_NewPos);
	OnNeedToUpdateMapperPawnPosition.Broadcast(l_YPosition - l_CurrentPosition);
}

void AC_Controller::UpdateNotesMaterial()
{
	if (UMMAConfig::Instance == nullptr) return;

	if (LeftNoteMaterial)
		delete LeftNoteMaterial;

	if (RightNoteMaterial)
		delete RightNoteMaterial;

	if (PassedLeftNoteMaterial)
		delete PassedLeftNoteMaterial;

	if (PassedRightNoteMaterial)
		delete PassedRightNoteMaterial;

	FLinearColor l_LeftColor = UMMAConfig::Instance->LeftEditorColor;
	FLinearColor l_RightColor = UMMAConfig::Instance->RightEditorColor;

	UMaterialInstanceDynamic* l_LeftNoteMaterial = UMaterialInstanceDynamic::Create(GlobalNoteMaterial, this);
	l_LeftNoteMaterial->SetVectorParameterValue("NoteColor", l_LeftColor);
	LeftNoteMaterial = l_LeftNoteMaterial;

	UMaterialInstanceDynamic* l_RightNoteMaterial = UMaterialInstanceDynamic::Create(GlobalNoteMaterial, this);
	l_RightNoteMaterial->SetVectorParameterValue("NoteColor", l_RightColor);
	RightNoteMaterial = l_RightNoteMaterial;

	UMaterialInstanceDynamic* l_PassedLeftNoteMaterial = UMaterialInstanceDynamic::Create(GlobalNoteMaterial, this);
	l_PassedLeftNoteMaterial->SetVectorParameterValue("NoteColor", l_LeftColor);
	l_PassedLeftNoteMaterial->SetScalarParameterValue("OpacityMultiplier", .5f);
	PassedLeftNoteMaterial = l_PassedLeftNoteMaterial;

	UMaterialInstanceDynamic* l_PassedRightNoteMaterial = UMaterialInstanceDynamic::Create(GlobalNoteMaterial, this);
	l_PassedRightNoteMaterial->SetVectorParameterValue("NoteColor", l_RightColor);
	l_PassedRightNoteMaterial->SetScalarParameterValue("OpacityMultiplier", .5f);
	PassedRightNoteMaterial = l_PassedRightNoteMaterial;

}

void AC_Controller::SetCurrentScene(FString sceneName)
{
	CurrentScene = sceneName;
}

float AC_Controller::GetBeat()
{
	return (PlayingTime / 60.f) * MapInfo.BPM;
}

float AC_Controller::GetPlayTime()
{
	return PlayingTime;
}

bool AC_Controller::IsPlaying()
{
	return Playing;
}

FMapData AC_Controller::GetMapData()
{
	return MapContent;
}

FString AC_Controller::GetCurrentSceneName()
{
	return CurrentScene;
}
