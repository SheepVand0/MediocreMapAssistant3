// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MapperPawn.h"

void AC_MapperPawn::ResetTransform()
{
	SetActorLocation(DEFAULT_PAWN_POSITION);
	SetActorRotation(DEFAULT_PAWN_ROTATION);
}

AC_MapperPawn::AC_MapperPawn() {
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	PawnMovement->MaxSpeed = PawnSpeed;

	SphereMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	SphereMeshComponent->SetupAttachment(Root);

	ConstructorHelpers::FObjectFinder<UStaticMesh> l_Sphere(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	SphereMeshComponent->SetStaticMesh(l_Sphere.Object);
	SphereMeshComponent->bOnlyOwnerSee = false;
	SphereMeshComponent->bOwnerNoSee = true;
	SphereMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	IsRightClickPressed = false;
	Speed = 1;

	EventsBound = false;
}

void AC_MapperPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AC_MapperPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveBackward", this, &AC_MapperPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AC_MapperPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveLeft", this, &AC_MapperPawn::MoveRight);

	PlayerInputComponent->BindAxis("MoveUp", this, &AC_MapperPawn::MoveUp);
	PlayerInputComponent->BindAxis("MoveDown", this, &AC_MapperPawn::MoveUp);

	PlayerInputComponent->BindAxis("CursorPrecision", this, &AC_MapperPawn::ChangeSpeed);

	PlayerInputComponent->BindAxis("LookControl", this, &AC_MapperPawn::RightClickValue);

	PlayerInputComponent->BindAxis("LookX", this, &AC_MapperPawn::LookX);
	PlayerInputComponent->BindAxis("LookY", this, &AC_MapperPawn::LookY);

	PlayerInputComponent->BindAction("Play", IE_Pressed, this, &AC_MapperPawn::PlayStop);
}

void AC_MapperPawn::BeginPlay() {
	Super::BeginPlay();
	SelectTool(AC_NoteTool::StaticClass());

	AC_Controller::Instance->OnNeedToUpdateMapperPawnPosition.AddDynamic(this, &AC_MapperPawn::OnNeedToUpdatePosition);
	AC_Controller::Instance->OnNeedToResetMapperPawnTransform.AddDynamic(this, &AC_MapperPawn::ResetTransform);
}

void AC_MapperPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (CurrentTool == nullptr) return;

	APlayerController* l_PlayerController = Cast<APlayerController>(GetController());
	FVector l_Position;
	FVector l_Direction;
	l_PlayerController->DeprojectMousePositionToWorld(l_Position, l_Direction);
	l_Position = l_Position + FVector(0, 0, 10);

	FHitResult l_Result;
	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		l_Position,
		GetActorLocation() + (l_Direction * 1000),
		TraceTypeQuery1,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		l_Result,
		false, FLinearColor::Red, FLinearColor::Green, 1.0f);

	if (l_Result.GetActor() == nullptr) return;

	

	if (l_Result.GetComponent()->ComponentHasTag("MappingGrid") && CurrentTool != nullptr) {
		int l_X = (FMath::Floor(l_Result.ImpactPoint.X / 25) * 25) + (25/2);
		int l_Z = FMath::Floor(l_Result.ImpactPoint.Z / 25) * 25 + (25/2);

		CurrentTool->SetActorLocation(FVector(l_X, l_Result.ImpactPoint.Y, l_Z));
	}
}

void AC_MapperPawn::MoveForward(float p_Value) {
	if (p_Value == 0.f || !IsRightClickPressed) return;

	if (Controller) {

		FRotator const ControlSpaceRot = Controller->GetControlRotation();

		// transform to world space and add it
		AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), p_Value * Speed);
	}
}

void AC_MapperPawn::MoveRight(float p_Value) {
	if (p_Value == 0.f || !IsRightClickPressed) return;

	if (Controller) {
		FRotator const ControlSpaceRot = Controller->GetControlRotation();

		// transform to world space and add it
		AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), p_Value * Speed);
	}
}

void AC_MapperPawn::MoveUp(float p_Value) {

	if (p_Value == 0.f || !IsRightClickPressed) return;

	AddMovementInput(FVector::UpVector, p_Value * Speed);
}

void AC_MapperPawn::LookX(float p_Value) {
	if (p_Value == 0.f || !IsRightClickPressed) return;

	AddControllerYawInput(p_Value);
}

void AC_MapperPawn::LookY(float p_Value) {
	if (p_Value == 0.f || !IsRightClickPressed) return;

	AddControllerPitchInput(p_Value);
}

void AC_MapperPawn::ChangeSpeed(float p_Value) {
	if (p_Value == 0.0f) return;

	if (!IsRightClickPressed) {
		if (AC_Controller::Instance != nullptr)
			AC_Controller::Instance->AddTime(100/(MappingDivision/4));
		return;
	}
	Speed += p_Value * 0.1f;
	if (Speed < 0.2f) Speed = 0;
	if (Speed > 4) Speed = 4;
	PawnMovement->MaxSpeed = PawnSpeed * Speed;
}

void AC_MapperPawn::RightClickValue(float value) {
	if (PlayerControllerReference == nullptr)
		PlayerControllerReference = Cast<APlayerController>(GetController());

	if (PlayerControllerReference == nullptr) return;

	if (value > 0.5f) {
		IsRightClickPressed = true;
		PlayerControllerReference->bShowMouseCursor = false;
	}
	else {
		IsRightClickPressed = false;
		PlayerControllerReference->bShowMouseCursor = true;
	}
}

void AC_MapperPawn::OnLeftClickUsed() {
	if (CurrentTool == nullptr) return;

	CurrentTool->OnUse(CurrentTool->GetActorLocation());
}

void AC_MapperPawn::OnNeedToUpdatePosition(float y) {
	auto l_Position = GetActorLocation();
	l_Position += FVector(0, y, 0);
	SetActorLocation(l_Position);
}

void AC_MapperPawn::PlayStop() {
	if (ControllerReference == nullptr) ControllerReference = Cast<AC_Controller>(UGameplayStatics::GetActorOfClass(GetWorld(), AC_Controller::StaticClass()));;

	if (IsRightClickPressed) return;

	if (!ControllerReference->IsPlaying()) ControllerReference->Play();
	else ControllerReference->Stop();
}

void AC_MapperPawn::SelectTool(TSubclassOf<AC_MappingTool> p_Tool) {
	if (CurrentTool != nullptr) {
		CurrentTool->Destroy();
	}

	CurrentTool = Cast<AC_MappingTool>(GetWorld()->SpawnActor(p_Tool));
	CurrentTool->SelectSubTool(0);
}

void AC_MapperPawn::OnCursorMovedOnOject() {

}