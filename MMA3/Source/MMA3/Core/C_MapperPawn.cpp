// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MapperPawn.h"

AC_MapperPawn::AC_MapperPawn() {

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	PawnMovement->MaxSpeed = 300;

	SphereMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	SphereMeshComponent->SetupAttachment(Root);

	ConstructorHelpers::FObjectFinder<UStaticMesh> l_Sphere(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	SphereMeshComponent->SetStaticMesh(l_Sphere.Object);
	SphereMeshComponent->bOnlyOwnerSee = false;
	SphereMeshComponent->bOwnerNoSee = true;
	SphereMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	IsRightClickPressed = false;
	Speed = 1;
}

void AC_MapperPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AC_MapperPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveBackward", this, &AC_MapperPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AC_MapperPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveLeft", this, &AC_MapperPawn::MoveRight);

	PlayerInputComponent->BindAxis("MoveUp", this, &AC_MapperPawn::MoveUp);
	PlayerInputComponent->BindAxis("MoveDown", this, &AC_MapperPawn::MoveUp);

	PlayerInputComponent->BindAction("ScrollUp", IE_Pressed, this, &AC_MapperPawn::SpeedUp);
	PlayerInputComponent->BindAction("ScrollDown", IE_Pressed, this, &AC_MapperPawn::SpeedDown);

	PlayerInputComponent->BindAction("RightClickPressed", IE_Pressed, this, &AC_MapperPawn::RightClickedPressed);
	PlayerInputComponent->BindAction("RightClickPressed", IE_Released, this, &AC_MapperPawn::RightClickedReleased);

	PlayerInputComponent->BindAxis("LookX", this, &AC_MapperPawn::LookX);
	PlayerInputComponent->BindAxis("LookY", this, &AC_MapperPawn::LookY);

	PlayerInputComponent->BindAction("Play", IE_Pressed, this, &AC_MapperPawn::PlayStop);
}

void AC_MapperPawn::BeginPlay() {
	Super::BeginPlay();
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

void AC_MapperPawn::SpeedUp() {
	if (Speed >= 2 || !IsRightClickPressed) return;
	Speed += 0.05f;
}

void AC_MapperPawn::SpeedDown() {
	if (Speed == 0 || !IsRightClickPressed) return;
	Speed -= 0.05f;
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

void AC_MapperPawn::PlayStop() {
	if (ControllerReference == nullptr) ControllerReference = Cast<AC_Controller>(UGameplayStatics::GetActorOfClass(GetWorld(), AC_Controller::StaticClass()));;

	if (!ControllerReference->Playing) ControllerReference->Play();
	else ControllerReference->Stop();
}