// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewsController.h"

// Sets default values
AViewsController::AViewsController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Instance = this;
}

// Called when the game starts or when spawned
void AViewsController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AViewsController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AViewsController::AddWidget(FString name, UUserWidget* x) {
	Widgets.Add(name, x);
}

UUserWidget* AViewsController::SetWidget(FString name) {
	TArray<FString> l_Keys;
	Widgets.GetKeys(l_Keys);

	if (!l_Keys.Contains(name)) {
		UE_LOG(LogTemp, Error, TEXT("Trying accessing null widget, name: %s"), *name);
		return nullptr;
	}

	UUserWidget* l_Widget = *Widgets.Find(name);

	NextWidget = l_Widget;
	if (CurrentWidget)
		CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (NextWidget)
		NextWidget->SetVisibility(ESlateVisibility::Visible);
	CurrentWidget = NextWidget;
	NextWidget = nullptr;

	//PlayAnimation();
	return l_Widget;
}

void AViewsController::CollapseAll()
{
	TArray<FString> l_Keys;
	Widgets.GetKeys(l_Keys);
	for (int l_i = 0; l_i < Widgets.Num(); l_i++) {
		UUserWidget* l_Widget = Widgets[l_Keys[l_i]];
		if (!l_Widget) continue;

		l_Widget->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void AViewsController::PlayAnimation() {
	UE_LOG(LogTemp, Display, TEXT("Playing widget animation"));
	FVector2D l_Size;
	GEngine->GameViewport->GetViewportSize(l_Size);
	float l_Width = l_Size.X;

	TArray<FAnimationKey*> l_Keys = { new FAnimationKey(0, l_Width), new FAnimationKey(0.5f, 0)};
	FFastAnimation* l_Animation = new FFastAnimation(
		FString("ViewsController"), 
		l_Keys, 
		SMethodPtr(this, &AViewsController::AnimationCallback).GetPointer(),
		nullptr//SMethodPtr(this, &AViewsController::AnimationFinished).GetPointer()
	);

	AFastAnimator::Instance->Play(l_Animation);
}

void AViewsController::AnimationCallback(float x) {
	FVector2D l_Size;
	GEngine->GameViewport->GetViewportSize(l_Size);
	float l_Width = l_Size.X;

	UE_LOG(LogTemp, Warning, TEXT("Animation Callback: %f"), x);

	if (CurrentWidget != nullptr) {
		auto l_RenderTransform = CurrentWidget->GetRenderTransform();
		l_RenderTransform.Translation.X = x - l_Width;
		CurrentWidget->SetRenderTransform(l_RenderTransform);
	}

	if (NextWidget != nullptr) {
		auto l_RenderTransform = NextWidget->GetRenderTransform();
		l_RenderTransform.Translation.X = x;
		NextWidget->SetRenderTransform(l_RenderTransform);
	}
}

void AViewsController::AnimationFinished()
{
	CurrentWidget = NextWidget;
	NextWidget = nullptr;
}
