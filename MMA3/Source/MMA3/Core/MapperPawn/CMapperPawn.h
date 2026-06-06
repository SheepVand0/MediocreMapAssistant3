// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "MMA3/Core/Controller/CController.h"
#include "Kismet/GameplayStatics.h"
#include "MMA3/Core/Tools/CMappingTool.h"
#include "MMA3/Core/Tools/Note/CNoteTool.h"
#include "Engine/GameViewportClient.h"
#include "Components/PrimitiveComponent.h"
#include "CMapperPawn.generated.h"


#define DEFAULT_PAWN_POSITION FVector(200, 200, 200)
#define DEFAULT_PAWN_ROTATION FRotator(0, -30.f, -130.f)

/**
 *
 */
UCLASS()
class MMA3_API ACMapperPawn : public APawn
{
	GENERATED_BODY()

public:

	static ACMapperPawn* Instance;

protected:

/////////////////////////////////////////////////////////////////////////////
/// Refs

	APlayerController* PlayerControllerReference;

	UPROPERTY()
	ACController* ControllerReference;

/////////////////////////////////////////////////////////////////////////////
/// Comps

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY()
	UFloatingPawnMovement* PawnMovement;

	UPROPERTY()
	UStaticMeshComponent* SphereMeshComponent;


/////////////////////////////////////////////////////////////////////////////
/// Input

	UPROPERTY()
	bool IsRightClickPressed;

	UPROPERTY()
	bool IsLeftCtrlPressed;

	UPROPERTY()
	bool IsLeftAltPressed;

	bool IsDotNoteInput;

/////////////////////////////////////////////////////////////////////////////
/// Data

	UPROPERTY()
	float Speed;

	float XAxis;
	float YAxis;

	TObjectPtr<AActor> AimedActor;

/////////////////////////////////////////////////////////////////////////////
/// Tool

	UPROPERTY()
	FVector ToolDirection;

	UPROPERTY(BlueprintReadOnly)
	float ToolAngle;

	UPROPERTY()
	TObjectPtr<ACMappingTool> CurrentTool;

	FVector LastToolDirection;

	FNoteData* LastNoteDirection;
	
	FVector TargetLocation;
	
	UFUNCTION()
	void ResetTransform();

/////////////////////////////////////////////////////////////////////////////
/// Input

	UFUNCTION(BlueprintCallable)
	void RightClickValue(float value);

	UFUNCTION()
	void OnLeftCtrlPressed();

	UFUNCTION()
	void OnLeftCtrlReleased();

	UFUNCTION()
	void OnLeftAltPressed();

	UFUNCTION()
	void OnLeftAltReleased();

	UFUNCTION()
	void OnLeftClickUsed();

	UFUNCTION(BlueprintCallable)
	void MoveForward(float p_Value);

	UFUNCTION(BlueprintCallable)
	void MoveRight(float p_Value);

	UFUNCTION(BlueprintCallable)
	void MoveUp(float p_Value);

	UFUNCTION(BlueprintCallable)
	void LookX(float p_Value);

	UFUNCTION(BlueprintCallable)
	void LookY(float p_Value);

	UFUNCTION()
	void InputTool(float x);

	UFUNCTION()
	void InputUndo();

	UFUNCTION()
	void InputDotNote();

	UFUNCTION()
	void InputDelete();

	UFUNCTION()
	void InputUnSelect();
	
	UFUNCTION() void FinishedSelection();
	
/////////////////////////////////////////////////////////////////////////////
/// Actions

	UFUNCTION()
	void SaveDifficulty();

	UFUNCTION()
	void DisplayEditorSettings();

	void SetToolDirection();

	UFUNCTION()
	void SwitchPrecision();
	
public:

	ACMapperPawn();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	static ACMapperPawn* GetMapperPawnRef();

/////////////////////////////////////////////////////////////////////////////
/// Data

	const float PawnSpeed = 300;

	bool IsSelecting = false;

	// Pos X on grid
	int CursorPosX;

	// Pos Z on grid
	int CursorPosZ;

	UPROPERTY()
	int SelectedMappingDivisionIndex = 0;

/////////////////////////////////////////////////////////////////////////////
/// Actions

	UFUNCTION() void IncreaseSpeed();

	UFUNCTION() void DecreaseSpeed();
	
	UFUNCTION() void ChangeSpeed(bool decrease);
	
	UFUNCTION() void PlayStop();

	UFUNCTION() void SelectTool(TSubclassOf<ACMappingTool> p_Tool);

	void UpdateLastNoteData();

/////////////////////////////////////////////////////////////////////////////
/// Events

	UFUNCTION()
	void OnCursorMovedOnOject();

	UFUNCTION()
	void OnNeedToUpdatePosition(float y);

};

ACMapperPawn* ACMapperPawn::Instance = nullptr;

static ACMapperPawn* GetMapperPawn() {
	return ACMapperPawn::Instance;
}