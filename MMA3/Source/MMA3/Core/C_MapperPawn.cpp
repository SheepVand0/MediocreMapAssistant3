// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MapperPawn.h"

AC_MapperPawn::AC_MapperPawn() {

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

	PlayerInputComponent->BindAction("RightClickPressed", IE_Pressed, this, &AC_MapperPawn::RightClickedPressed);
	PlayerInputComponent->BindAction("RightClickPressed", IE_Released, this, &AC_MapperPawn::RightClickedReleased);

	PlayerInputComponent->BindAxis("LookX", this, &AC_MapperPawn::LookX);
	PlayerInputComponent->BindAxis("LookY", this, &AC_MapperPawn::LookY);

	PlayerInputComponent->BindAction("Play", IE_Pressed, this, &AC_MapperPawn::PlayStop);
}

void AC_MapperPawn::BeginPlay() {
	Super::BeginPlay();
	SelectTool(AC_NoteTool::StaticClass());
}

void AC_MapperPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!EventsBound) {
		if (AC_Controller::Instance == nullptr) return;

		EventsBound = true;
	}

	if (CurrentTool == nullptr) return;

	APlayerController* l_PlayerController = Cast<APlayerController>(GetController());
	FVector l_Position;
	FVector l_Direction;
	l_PlayerController->DeprojectMousePositionToWorld(l_Position, l_Direction);

	FHitResult l_Result;
	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + (l_Direction * 1000),
		TraceTypeQuery1,
		true,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		l_Result,
		true);

	AC_Controller* l_Controller = Cast<AC_Controller>(l_Result.GetActor());
	if (l_Controller == nullptr) return;

	if (l_Controller->MappingGrid == l_Result.GetComponent()) {
		int l_X = FMath::Floor(l_Result.ImpactPoint.X / 25) * 25;
		int l_Z = FMath::Floor(l_Result.ImpactPoint.Z / 25) * 25;

		CurrentTool->SetActorLocation(FVector(l_X, l_Result.ImpactPoint.Z, l_Z));
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
			AC_Controller::Instance->AddTime(p_Value * 0.1f);
		return;
	}
	Speed += p_Value * 0.1f;
	if (Speed < 0.2f) Speed = 0;
	if (Speed > 4) Speed = 4;
	PawnMovement->MaxSpeed = PawnSpeed * Speed;
}

void AC_MapperPawn::RightClickedPressed() {
	IsRightClickPressed = true;
	APlayerController* l_Controller = Cast<APlayerController>(GetController());
	if (l_Controller != nullptr) l_Controller->bShowMouseCursor = false;
}

void AC_MapperPawn::RightClickedReleased() {
	IsRightClickPressed = false;
	APlayerController* l_Controller = Cast<APlayerController>(GetController());
	if (l_Controller != nullptr) l_Controller->bShowMouseCursor = true;
}

void AC_MapperPawn::OnLeftClickUsed() {
	if (CurrentTool == nullptr) return;

	CurrentTool->OnUse(CurrentTool->GetActorLocation());
}

void AC_MapperPawn::PlayStop() {
	if (ControllerReference == nullptr) ControllerReference = Cast<AC_Controller>(UGameplayStatics::GetActorOfClass(GetWorld(), AC_Controller::StaticClass()));;

	if (IsRightClickPressed) return;

	if (!ControllerReference->Playing) ControllerReference->Play();
	else ControllerReference->Stop();
}

void AC_MapperPawn::SelectTool(TSubclassOf<AC_MappingTool> p_Tool) {
	if (CurrentTool != nullptr) {
		CurrentTool->Destroy();
	}

	CurrentTool = Cast<AC_MappingTool>(GetWorld()->SpawnActor(p_Tool));
}

void AC_MapperPawn::OnCursorMovedOnOject() {

}