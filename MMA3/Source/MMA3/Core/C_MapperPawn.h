// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "C_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "C_MappingTool.h"
#include "Tools/C_NoteTool.h"
#include "Engine/GameViewportClient.h"
#include "C_MapperPawn.generated.h"

/**
 *
 */
UCLASS()
class MMA3_API AC_MapperPawn : public APawn
{
	GENERATED_BODY()

public:

	AC_MapperPawn();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		UFloatingPawnMovement* PawnMovement;

	UPROPERTY()
		USceneComponent* Root;

	UPROPERTY()
		UStaticMeshComponent* SphereMeshComponent;

	UPROPERTY()
		AC_Controller* ControllerReference;

	UPROPERTY()
		bool IsRightClickPressed;

	UPROPERTY()
		AC_MappingTool* CurrentTool;

	UPROPERTY()
		float Speed;

	UPROPERTY()
		float PawnSpeed = 300;

	bool EventsBound;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

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

	UFUNCTION(BlueprintCallable)
		void ChangeSpeed(float Value);

	UFUNCTION(BlueprintCallable)
		void RightClickedPressed();

	UFUNCTION(BlueprintCallable)
		void RightClickedReleased();

	UFUNCTION()
		void OnLeftClickUsed();

	UFUNCTION()
		void PlayStop();

	UFUNCTION()
		void SelectTool(TSubclassOf<AC_MappingTool> p_Tool);

	UFUNCTION()
		void OnCursorMovedOnOject();
};
