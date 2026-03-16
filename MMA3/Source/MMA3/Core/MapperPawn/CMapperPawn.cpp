// Fill out your copyright notice in the Description page of Project Settings.


#include "CMapperPawn.h"
#include "MMA3/Core/Tools/Wall/CWallTool.h"
#include "MMA3/Core/Controller/CController.h"

void ACMapperPawn::ResetTransform()
{
	SetActorLocation(DEFAULT_PAWN_POSITION);
	SetActorRotation(DEFAULT_PAWN_ROTATION);
}

ACMapperPawn::ACMapperPawn() {
	ConstructorHelpers::FObjectFinder<UStaticMesh> l_Sphere(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	PawnMovement->MaxSpeed = PawnSpeed;

	SphereMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	SphereMeshComponent->SetupAttachment(Root);
	SphereMeshComponent->SetStaticMesh(l_Sphere.Object);
	SphereMeshComponent->bOnlyOwnerSee = false;
	SphereMeshComponent->bOwnerNoSee = true;
	SphereMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	IsRightClickPressed = false;
	Speed = 1;
}

ACMapperPawn* ACMapperPawn::GetMapperPawnRef()
{
	return Instance;
}

void ACMapperPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACMapperPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACMapperPawn::MoveRight);

	PlayerInputComponent->BindAxis("MoveUp", this, &ACMapperPawn::MoveUp);
	PlayerInputComponent->BindAxis("MoveDown", this, &ACMapperPawn::MoveUp);

	PlayerInputComponent->BindAction("ScrollUp", IE_Pressed, this, &ACMapperPawn::IncreaseSpeed);
	PlayerInputComponent->BindAction("ScrollDown", IE_Pressed, this, &ACMapperPawn::DecreaseSpeed);

	PlayerInputComponent->BindAxis("LookControl", this, &ACMapperPawn::RightClickValue);

	PlayerInputComponent->BindAxis("LookX", this, &ACMapperPawn::LookX);
	PlayerInputComponent->BindAxis("LookY", this, &ACMapperPawn::LookY);

	PlayerInputComponent->BindAction("Play", IE_Pressed, this, &ACMapperPawn::PlayStop);

	PlayerInputComponent->BindAction("Trigger", IE_Pressed, this, &ACMapperPawn::OnLeftClickUsed);

	PlayerInputComponent->BindAction("Save", IE_Pressed, this, &ACMapperPawn::SaveDifficulty);

	PlayerInputComponent->BindAction("EditorEscape", IE_Pressed, this, &ACMapperPawn::DisplayEditorSettings);

	PlayerInputComponent->BindAxis("Tools", this, &ACMapperPawn::InputTool);

	PlayerInputComponent->BindAction("SwapPrecisionPanel", IE_Pressed, this, &ACMapperPawn::SwitchPrecision);

	PlayerInputComponent->BindAction("LeftCtrl", IE_Pressed, this, &ACMapperPawn::OnLeftCtrlPressed);
	PlayerInputComponent->BindAction("LeftCtrl", IE_Released, this, &ACMapperPawn::OnLeftCtrlReleased);

	PlayerInputComponent->BindAction("LeftAlt", IE_Pressed, this, &ACMapperPawn::OnLeftAltPressed);
	PlayerInputComponent->BindAction("LeftAlt", IE_Released, this, &ACMapperPawn::OnLeftAltReleased);

	PlayerInputComponent->BindAction("Undo", IE_Pressed, this, &ACMapperPawn::InputUndo);
	PlayerInputComponent->BindAction("DotNote", IE_Pressed, this, &ACMapperPawn::InputDotNote);

	PlayerInputComponent->BindAction("Delete", IE_Pressed, this, &ACMapperPawn::InputDelete);

	PlayerInputComponent->BindAction("UnSelect", IE_Pressed, this, &ACMapperPawn::InputUnSelect);

}

void ACMapperPawn::BeginPlay() {
	Super::BeginPlay();
	Instance = this;

	SelectTool(ACNoteTool::StaticClass());

	ToolDirection = FVector(0, 1, 0);
	SetToolDirection();

	GetMappingController()->OnNeedToUpdateMapperPawnPosition.AddDynamic(this, &ACMapperPawn::OnNeedToUpdatePosition);
	GetMappingController()->OnNeedToResetMapperPawnTransform.AddDynamic(this, &ACMapperPawn::ResetTransform);

	InputComponent->SetActive(false);
}

void ACMapperPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!GetMappingController()->GetMapData()) return;

	if (!IsRightClickPressed) {
		if (XAxis == 0 && YAxis == 0) {
			ToolDirection = LastToolDirection;
			SetToolDirection();
		}

		if (XAxis == 0 && YAxis != 0) {
			ToolDirection.X = 0;
			ToolDirection.Y = YAxis;
			SetToolDirection();
		}

		if (YAxis == 0 && XAxis != 0) {
			ToolDirection.Y = 0;
			ToolDirection.X = XAxis;
			SetToolDirection();
		}

		if (YAxis != 0 && XAxis != 0) {
			ToolDirection.X = XAxis;
			ToolDirection.Y = YAxis;
			SetToolDirection();
		}
	}

	if (CurrentTool) {

		APlayerController* l_PlayerController = Cast<APlayerController>(GetController());
		FVector l_Position;
		FVector l_Direction;
		l_PlayerController->DeprojectMousePositionToWorld(l_Position, l_Direction);

		/// Trace mouse on world
		FHitResult l_Result;
		UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			l_Position,
			l_Position + (l_Direction * 15000),
			TraceTypeQuery2,
			false,
			TArray<AActor*>(),
			EDrawDebugTrace::None,
			l_Result,
			false, FLinearColor::Red, FLinearColor::Green, 1.0f);

		if (l_Result.GetActor() == nullptr) {
			AimedActor = nullptr;
			return;
		}

		int l_X = (FMath::Floor(l_Result.ImpactPoint.X / 25) * 25) + (25 / 2);
		int l_Z = FMath::Floor(l_Result.ImpactPoint.Z / 25) * 25 + (25 / 2);

		CursorPosX = l_X;
		CursorPosZ = l_Z;

		if (IsLeftAltPressed) {
			ACNote* l_Note = Cast<ACNote>(AimedActor);
			if (l_Note) {
				int l_PreX = l_Note->NoteData->Line;
				int l_PreY = l_Note->NoteData->Layer;
					
				l_Note->NoteData->Line = -((l_X + 13) / 25) + 4;
				l_Note->NoteData->Layer = (l_Z) / 25;
				l_Note->SetActorRelativeLocation(ACNote::CalculateNoteLocation(*l_Note->NoteData));

				/*if (l_PreX != l_Note->NoteData->Line || l_Note->NoteData->Layer != l_PreY)*/
					//GetMappingController()->ResetCurrent();
			}
		}
		else {
			AimedActor = l_Result.GetActor();
		}

		if (l_Result.GetComponent()) {
			if (l_Result.GetComponent()->ComponentHasTag("MappingGrid") && CurrentTool != nullptr) {

				CurrentTool->SetActorLocation(FVector(l_X, l_Result.ImpactPoint.Y, l_Z));
			}
		}
	}
}

void ACMapperPawn::MoveForward(float p_Value) {


	YAxis = p_Value;
	if (YAxis)
		IsDotNoteInput = false;

	if (p_Value && IsRightClickPressed) {
		if (Controller) {

			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), p_Value * Speed);
		}
	}
}

void ACMapperPawn::MoveRight(float p_Value) {

	XAxis = p_Value;
	if (XAxis)
		IsDotNoteInput = false;

	if (p_Value && IsRightClickPressed) {
		if (Controller) {
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), p_Value * Speed);
		}
	}
}

void ACMapperPawn::MoveUp(float p_Value) {
	if (p_Value && IsRightClickPressed) {
		AddMovementInput(FVector::UpVector, p_Value * Speed);
	}
}

void ACMapperPawn::LookX(float p_Value) {
	if (p_Value && IsRightClickPressed) {
		AddControllerYawInput(p_Value);
	}
}

void ACMapperPawn::LookY(float p_Value) {
	if (p_Value && IsRightClickPressed) {
		AddControllerPitchInput(p_Value);
	}
}

void ACMapperPawn::SaveDifficulty()
{
	GetMappingController()->Save();
}

void ACMapperPawn::DisplayEditorSettings()
{
	GetMappingController()->DisplayEditModeSettings();
}

void ACMapperPawn::SetToolDirection()
{
	ACNoteTool* l_Tool = Cast<ACNoteTool>(CurrentTool);

	if (!l_Tool) return;
	if (l_Tool->Type == 3) return;

	FVector l_Dir = ToolDirection;

	if (l_Dir == FVector::Zero()) return;

	if (IsDotNoteInput) {
		l_Tool->Direction = 8;
		l_Tool->SetActorRotation(FRotator(0, -90, 0));

		LastToolDirection = FVector::Zero();

		return;
	}

	l_Dir.Normalize();

	int l_Angle = ((l_Dir.Y < 0 ? -1 : 1) * FMath::RadiansToDegrees(FMath::Acos(l_Dir.X))) - 90;

	/// Fixing precision loss resulting getting 44 degree instead of 45
	int l_Mod = (l_Angle % 45);
	if (l_Mod != 0) {
		if (l_Angle > 0)
			l_Angle += (45 - l_Mod);
		else
			l_Angle -= (45 + l_Mod);
	}


	ToolAngle = l_Angle % 360;
	
	/*if (GetMappingController()->GetMapData()) {
		FNoteData* l_LastNote = GetMappingController()->GetLastSameColorNote(GetMappingController()->GetBeat(), l_Tool->Type);

		if (l_LastNote) {
			auto l_LastAngle = *ACNote::RotationByCutDirection.Find(l_LastNote->Direction);
			if (l_LastAngle) {
				if (abs(l_Angle - l_LastAngle) <= 90) {
					if (l_Angle <= l_LastAngle)
						l_Angle = l_LastAngle - 90;
					else
						l_Angle = l_LastAngle + 90;
				}
			}
		}
	}*/

	l_Tool->Direction = ACNote::CutDirectionFromAngle(l_Angle);
	//UE_LOG(LogTemp, Display, TEXT("Angle: %d"), l_Tool->Direction);
	l_Tool->SetActorRotation(FRotator(0, -90, l_Angle));

	LastToolDirection = ToolDirection;
}

void ACMapperPawn::InputTool(float x)
{
	if (x == 0) return;

	if (x == 1 || x == 2 || x == 3) {
		ACNoteTool* l_NoteTool = nullptr;
		if ((l_NoteTool = Cast<ACNoteTool>(CurrentTool)) == nullptr) {
			SelectTool(ACNoteTool::StaticClass());
			l_NoteTool = Cast<ACNoteTool>(CurrentTool);
		}

		switch ((int)x) {
		case 1:
			l_NoteTool->Type = 0;
			break;
		case 2:
			l_NoteTool->Type = 1;
			break;
		case 3:
			l_NoteTool->Type = 3;
			break;
		}

		CurrentTool->Update();
	}
	else
		if (x == 4) {
			SelectTool(ACWallTool::StaticClass());
		}
}

void ACMapperPawn::SwitchPrecision()
{
	SelectedMappingDivisionIndex += 1;
	if (SelectedMappingDivisionIndex > GetConfig()->MappingDivisions.Num() - 1) {
		SelectedMappingDivisionIndex = 0;
	}
	GetMappingController()->EditModeWidget->UpdateSelectedMappingDivision();
}

void ACMapperPawn::InputUndo()
{
	if (!IsLeftCtrlPressed) return;

	UE_LOG(LogTemp, Display, TEXT("Input done"));

	GetMappingController()->History.Undo();
}

void ACMapperPawn::InputDotNote()
{
	IsDotNoteInput = true;

	if (ACNoteTool* l_Tool = Cast<ACNoteTool>(CurrentTool)) {
		l_Tool->Direction = 8;
		l_Tool->Update();
	}
}

void ACMapperPawn::InputDelete()
{

	if (GetMappingController()->SelectionEndBPM != GetMappingController()->SelectionFirstBPM) {

		for (int x = 0; x < GetMappingController()->MapContent->_notes.Num(); x++) {
			float l_B = GetMappingController()->MapContent->_notes[x]->Beat;

			if (l_B <= GetMappingController()->SelectionEndBPM && l_B >= GetMappingController()->SelectionFirstBPM) {
				GetMappingController()->MapContent->_notes.RemoveAt(x);
				x--;
			}
		}

		for (int x = 0; x < GetMappingController()->MapContent->_walls.Num(); x++) {
			float l_B = GetMappingController()->MapContent->_walls[x]->Beat;

			if (l_B <= GetMappingController()->SelectionEndBPM && l_B >= GetMappingController()->SelectionFirstBPM) {
				GetMappingController()->MapContent->_walls.RemoveAt(x);
				x--;
			}
		}

		GetMappingController()->ResetCurrent();

		return;
	}

	if (ACNote* l_Note = Cast<ACNote>(AimedActor)) {
		GetMappingController()->MapContent->_notes.Remove(l_Note->NoteData);
		GetObjectsSpawner()->SpawnObjects(GetMappingController()->MapContent, GetMappingController()->GetBeatCell(), 0.0f, 0.0f);
		GetMappingController()->UpdateBeatGrid();
	}
	else if (ACWall* l_Wall = Cast<ACWall>(AimedActor)) {
		GetMappingController()->MapContent->_walls.Remove(l_Wall->WallData);
		GetObjectsSpawner()->SpawnObjects(GetMappingController()->MapContent, GetMappingController()->GetBeatCell(), 0.0f, 0.0f);
		GetMappingController()->UpdateBeatGrid();
	}
}

void ACMapperPawn::InputUnSelect()
{
	GetMappingController()->SelectionFirstBPM = 0.f;
	GetMappingController()->SelectionEndBPM = 0.f;
}

void ACMapperPawn::RightClickValue(float value) {
	if (PlayerControllerReference == nullptr) {
		PlayerControllerReference = Cast<APlayerController>(GetController());
	}

	if (PlayerControllerReference == nullptr) {
		return;
	}

	if (value > 0.5f) {
		IsRightClickPressed = true;
		PlayerControllerReference->bShowMouseCursor = false;
	}
	else {
		IsRightClickPressed = false;
		PlayerControllerReference->bShowMouseCursor = true;
	}
}

void ACMapperPawn::OnLeftCtrlPressed()
{
	IsLeftCtrlPressed = true;
}

void ACMapperPawn::OnLeftCtrlReleased()
{
	IsLeftCtrlPressed = false;
}

void ACMapperPawn::OnLeftAltPressed()
{
	IsLeftAltPressed = true;
}

void ACMapperPawn::OnLeftAltReleased()
{
	IsLeftAltPressed = false;
}

void ACMapperPawn::OnLeftClickUsed() {
	//if (CurrentTool == nullptr) return;

	if (!GetMappingController()->GetMapData()) return;

	UE_LOG(LogTemp, Display, TEXT("clicked"));

	if (IsSelecting) {
		IsSelecting = false;
		return;
	}

	if (IsLeftCtrlPressed) {
		if (IsSelecting) {
			IsSelecting = false;
		}
		else {
			IsSelecting = true;
			GetMappingController()->SelectionFirstBPM = GetMappingController()->GetBeat();
		}
	}
	else {
		if (CurrentTool) {
			CurrentTool->OnUse(CurrentTool->GetActorLocation());

			UpdateLastNoteData();
		}
	}

}

void ACMapperPawn::IncreaseSpeed()
{
	if (!GetMappingController()->GetMapData()) return;

	if (IsRightClickPressed) {
		Speed *= 1.1f;
		if (Speed > 4)
			Speed = 4;
	}
	else if (IsLeftCtrlPressed)
	{
		GetConfig()->MappingDivisions[SelectedMappingDivisionIndex] *= 2;
		if (GetConfig()->MappingDivisions[SelectedMappingDivisionIndex] > 128) {
			GetConfig()->MappingDivisions[SelectedMappingDivisionIndex] = 128;
		}
		UMMAConfig::SaveConfig(GetConfig());
		GetMappingController()->EditModeWidget->UpdateSelectedMappingDivision();
	}
	else
	{
		if (!GetMappingController()->IsPlaying()) {
			float l_Time = GetMappingController()->BeatToTime(GetMappingController()->GetBeat() + (1.f / GetConfig()->MappingDivisions[SelectedMappingDivisionIndex]));
			GetMappingController()->SetTime(l_Time);
		}
	}
}

void ACMapperPawn::DecreaseSpeed()
{
	if (!GetMappingController()->GetMapData()) return;

	if (IsRightClickPressed) {
		Speed *= 0.9f;
		if (Speed < 0.5f)
			Speed = 0.5f;
	}
	else if (IsLeftCtrlPressed) {
		GetConfig()->MappingDivisions[SelectedMappingDivisionIndex] /= 2;
		if (GetConfig()->MappingDivisions[SelectedMappingDivisionIndex] <= 1) {
			GetConfig()->MappingDivisions[SelectedMappingDivisionIndex] = 1;
		}
		UMMAConfig::SaveConfig(GetConfig());
		GetMappingController()->EditModeWidget->UpdateSelectedMappingDivision();
	}
	else
	{
		if (!GetMappingController()->IsPlaying()) {
			float l_Time = GetMappingController()->BeatToTime(GetMappingController()->GetBeat() - (1.f / GetConfig()->MappingDivisions[SelectedMappingDivisionIndex]));
			GetMappingController()->SetTime(l_Time);
		}
	}
}

void ACMapperPawn::OnNeedToUpdatePosition(float y) {
	auto l_Position = GetActorLocation();
	l_Position += FVector(0, y, 0);
	SetActorLocation(l_Position);
}

void ACMapperPawn::PlayStop() {
	if (ControllerReference == nullptr) ControllerReference = Cast<ACController>(UGameplayStatics::GetActorOfClass(GetWorld(), ACController::StaticClass()));;

	if (IsRightClickPressed) return;

	if (!ControllerReference->IsPlaying()) ControllerReference->Play();
	else {
		ControllerReference->Stop();

		float l_Beat = GetMappingController()->GetBeat();
		float l_MappingDivision = 1.0f / GetConfig()->MappingDivisions[SelectedMappingDivisionIndex];
		while (l_Beat > l_MappingDivision)
			l_Beat -= l_MappingDivision;

		/*UE_LOG(LogTemp, Display, TEXT("l_BeatDiff: %f"), l_Beat);
		UE_LOG(LogTemp, Display, TEXT("l_Beat: %f"), GetMappingController()->GetBeat())*/

		if (l_Beat != 0) {
			GetMappingController()->SetTime(GetMappingController()->BeatToTime(GetMappingController()->GetBeat() - l_Beat));
		}
	}
}

void ACMapperPawn::SelectTool(TSubclassOf<ACMappingTool> toolClass) {
	if (CurrentTool != nullptr) {
		CurrentTool->Destroy();
	}

	FActorSpawnParameters l_SpawnParams;
	l_SpawnParams.bNoFail = true;
	CurrentTool = GetWorld()->SpawnActor<ACMappingTool>(toolClass, l_SpawnParams);
	CurrentTool->Update();
	CurrentTool->SetActorScale3D(CurrentTool->GetCustomScale());
}

void ACMapperPawn::OnCursorMovedOnOject() {

}

void ACMapperPawn::UpdateLastNoteData()
{
	LastNoteDirection = GetMappingController()->GetLastSameColorNote(GetMappingController()->GetBeat(), Cast<ACNoteTool>(CurrentTool) ? Cast<ACNoteTool>(CurrentTool)->Type : -1);
}
