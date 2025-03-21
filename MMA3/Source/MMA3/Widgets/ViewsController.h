// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "MMA3/FastAnimator.h"
#include "MMA3/Misc/SActionPtr.h"
#include "ViewsController.generated.h"

UCLASS()
class MMA3_API AViewsController : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AViewsController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AnimationCallback(float x);

public:
	static AViewsController* Instance;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TMap<FString, UUserWidget*> Widgets;

	UPROPERTY()
		UUserWidget* CurrentWidget;

	UPROPERTY()
		UUserWidget* NextWidget;

	UFUNCTION(BlueprintCallable)
		void AddWidget(FString name, UUserWidget* x);

	UFUNCTION(BlueprintCallable)
		UUserWidget* SetWidget(FString name);

	UFUNCTION()
		void PlayAnimation();
};

AViewsController* AViewsController::Instance = nullptr;