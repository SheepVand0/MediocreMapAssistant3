// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
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

	UPROPERTY()
		UFloatingPawnMovement* PawnMovement;

	UPROPERTY()
		USceneComponent* Root;

	UPROPERTY()
		UStaticMeshComponent* SphereMeshComponent;

	UPROPERTY()
		bool IsRightClickPressed;

	UPROPERTY()
		float Speed;

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
		void SpeedUp();

	UFUNCTION(BlueprintCallable)
		void SpeedDown();

	UFUNCTION(BlueprintCallable)
		void RightClickedPressed();

	UFUNCTION(BlueprintCallable)
		void RightClickedReleased();

};
