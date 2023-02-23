// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "MMA3/Widgets/MainMenu/LevelSelectionWidget.h"
#include "MMA3/Widgets/MainMenu/MapDetailsWidget.h"
#include "C_Controller.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReady);

UCLASS()
class MMA3_API AC_Controller : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AC_Controller();

	UPROPERTY(EditAnywhere)
		ULevelSelectionWidget* m_MapSelectionWidget;

	UPROPERTY(EditAnywhere)
		UMapDetailsWidget* m_MapDetailsWidget;

	UPROPERTY(EditAnywhere)
		FOnReady OnControllerReady;

	UPROPERTY(EditAnywhere)
		APlayerController* PlayerControllerReference;

	UPROPERTY()
		USceneComponent* BeatCellsRoot;

	UPROPERTY()
		FString CurrentScene = "MainMenu";

	UPROPERTY()
		FMapData CurrentMapData;

	UPROPERTY()
		float PlayingTime;

	UPROPERTY()
		bool Playing;

	UFUNCTION()
		void GenerateGrid(FMapInfo p_Info);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
