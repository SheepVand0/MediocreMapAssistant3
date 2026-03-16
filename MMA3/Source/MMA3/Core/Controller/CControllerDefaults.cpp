#include "CController.h"
#include "MMA3/Core/MapperPawn/CMapperPawn.h"

ACController::ACController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bHighPriority = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>l_Cube(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>l_NoteMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/NoteBody_Cube.NoteBody_Cube'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>l_Bomb(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/Bomb.Bomb'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>l_Dot(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/NoteBody_Cylinder_001.NoteBody_Cylinder_001'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>l_MappingGridMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/MappingGridPlane.MappingGridPlane'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance>l_CubeMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/Mapping/M_BasicWhiteShape.M_BasicWhiteShape'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance>l_BombMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/Mapping/M_Bomb.M_Bomb'"));
	ConstructorHelpers::FObjectFinder<USoundWave> l_HitSound(TEXT("/Script/Engine.SoundWave'/Game/Assets/Sounds/HitSounds/HitSoundb.HitSoundb'"));
	ConstructorHelpers::FObjectFinder<UMaterial> l_WallMaterial(TEXT("/Script/Engine.Material'/Game/Assets/Materials/Mapping/M_Obstacle.M_Obstacle'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance>l_NoteMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/Mapping/M_NoteInstance.M_NoteInstance'"));
	ConstructorHelpers::FObjectFinder<UMaterial>l_SoundVisMaterial(TEXT("/Script/Engine.Material'/Game/Assets/Materials/Waveform/M_WaveForm.M_WaveForm'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance>l_ObjectOutlineMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/Outlines/MNoteOutline_Inst.MNoteOutline_Inst'"));
	ConstructorHelpers::FObjectFinder<UMaterial>l_GridOutlineMaterial(TEXT("/Script/Engine.Material'/Game/Assets/Materials/Outlines/MGridOutline.MGridOutline'"));

	GridOutlineMaterial = l_GridOutlineMaterial.Object;

	OutlineMaterial = l_ObjectOutlineMaterial.Object;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));

	PlayingTime = 0;
	ActorTime = 0;

	HitSound = l_HitSound.Object;

	MappingRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Mapping Root"));

	TimeMarkerCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	TimeMarkerCube->SetStaticMesh(l_Cube.Object);
	TimeMarkerCube->SetRelativeScale3D(FVector(1, 0.05f, 0.05f));
	TimeMarkerCube->SetMaterial(0, l_CubeMat.Object);
	TimeMarkerCube->SetupAttachment(MappingRoot);

	Instance = this;

	GlobalNoteMaterial = l_NoteMaterial.Object;
	BombMesh = l_Bomb.Object;
	DotMesh = l_Dot.Object;
	CubeMesh = l_NoteMesh.Object;
	BombMaterial = l_BombMat.Object;

	MappingGrid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mapping Grid"));
	MappingGrid->SetStaticMesh(l_MappingGridMesh.Object);
	MappingGrid->SetCollisionObjectType(ECC_WorldStatic);
	MappingGrid->ComponentTags.Add("MappingGrid");
	MappingGrid->SetupAttachment(MappingRoot);

	SoundvisMaterial = l_SoundVisMaterial.Object;

	SoundVisMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("SoundVisMesh Mesh"));
	SoundVisMesh->SetupAttachment(MappingRoot);
	SoundVisMesh->SetMaterial(0, SoundvisMaterial);
	SoundVisMesh->SetRelativeScale3D(FVector(1, -1, 1));
	SoundVisMesh->SetRelativeLocation(FVector(60, 0, 0));
	SoundVisMesh->SetRelativeRotation(FRotator(0, 90, 0));

	HitSoundAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Hit Sound Audio Component"));
	//HitSoundAudioComponent->SetupAttachment(GetRootComponent());
	HitSoundAudioComponent->Sound = HitSound;
	HitSoundAudioComponent->bAlwaysPlay = false;
	HitSoundAudioComponent->bAutoActivate = false;

	WallMaterial = l_WallMaterial.Object;

	RenderDistance = 15;

	Instance = this;
}

// Sets default values

void ACController::OnConstruction(const FTransform& transform)
{
}

// Called when the game starts or when spawned
void ACController::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine) {
		GEngine->SetGarbageEliminationEnabled(false);
	}

	PlayerControllerReference = GetWorld()->GetFirstPlayerController();

	PlayerControllerReference->bShowMouseCursor = true;

	HitSoundAudioComponent->Stop();

	if (!UGameplayStatics::DoesSaveGameExist(MMA_SAVE_GAME_SLOT_NAME, 0)) {
		UGameplayStatics::SaveGameToSlot(UGameplayStatics::CreateSaveGameObject(UMMAConfig::StaticClass()), MMA_SAVE_GAME_SLOT_NAME, 0);
	}
	Config = Cast<UMMAConfig>(UGameplayStatics::LoadGameFromSlot(MMA_SAVE_GAME_SLOT_NAME, 0));

	OnNeedToCreateWidgets.Broadcast();
	
	TArray<UUserWidget*> l_FoundWidgets;

	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), l_FoundWidgets, UEditModeWidget::StaticClass(), false);
	EditModeWidget = Cast<UEditModeWidget>(l_FoundWidgets[0]);
	EditModeWidget->AddToViewport();

	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), l_FoundWidgets, ULevelSelectionWidget::StaticClass(), false);
	LevelSelectionWidget = Cast<ULevelSelectionWidget>(l_FoundWidgets[0]);
	LevelSelectionWidget->AddToViewport();

	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), l_FoundWidgets, UMapDetailsWidget::StaticClass(), false);
	MapDetailsWidget = Cast<UMapDetailsWidget>(l_FoundWidgets[0]);
	MapDetailsWidget->AddToViewport();

	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), l_FoundWidgets, UMainSettings::StaticClass(), false);
	MainSettingsWidget = Cast<UMainSettings>(l_FoundWidgets[0]);
	MainSettingsWidget->AddToViewport();

	GetViewsController()->AddWidget("EditModeWidget", EditModeWidget);
	GetViewsController()->AddWidget("LevelSelectionWidget", LevelSelectionWidget);
	GetViewsController()->AddWidget("MapDetailsWidget", MapDetailsWidget);
	GetViewsController()->AddWidget("MainSettingsWidget", MainSettingsWidget);

	GetViewsController()->CollapseAll();

	GetViewsController()->SetWidget("LevelSelectionWidget");

	LevelSelectionWidget->Init();

	AudioComponent->VolumeMultiplier = GetConfig()->SoundVolume;
}

void ACController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	ActorTime += deltaTime;

	if (Playing) {
		PlayingTime = ActorTime - StartedPlayTime;

		if (PlayingTime >= MapInfo->Song->GetDuration()) {
			PlayingTime = MapInfo->Song->GetDuration();
			Stop();
		}

		UpdateBeatGrid();
		UpdateVisMesh();

		this->DeltaTime = PlayingTime - LastPlayingTime;

		LastPlayingTime = PlayingTime;

		GetMapperPawn()->UpdateLastNoteData();
	}

	EditModeWidget->UpdateWidget(PlayingTime, Playing);

	if (GetMapperPawn()->IsSelecting) {
		SelectionEndBPM = GetBeat();
	}

	if (MapContent) {
		if (FMath::Clamp(GetBeat(), SelectionFirstBPM, SelectionEndBPM) == GetBeat() && SelectionFirstBPM != SelectionEndBPM) {
			MappingGrid->SetOverlayMaterial(GridOutlineMaterial);
		}
		else {
			MappingGrid->SetOverlayMaterial(nullptr);
		}
	}
}