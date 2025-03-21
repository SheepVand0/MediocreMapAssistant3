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
	UUserWidget* l_Widget = *Widgets.Find(name);
	if (l_Widget == nullptr) return nullptr;

	NextWidget = l_Widget;
	return l_Widget;
}

void AViewsController::PlayAnimation() {
	FVector2D l_Size;
	GEngine->GameViewport->GetViewportSize(l_Size);
	float l_Width = l_Size.X;

	TArray<FAnimationKey*> l_Keys = { new FAnimationKey(0, l_Width), new FAnimationKey(0.5f, 0)};
	FFastAnimation* l_Animation = new FFastAnimation(FString("ViewController"), l_Keys, SMethodPtr(this, &AViewsController::AnimationCallback).GetPointer());

	AFastAnimator::Instance->Play(l_Animation);
}

void AViewsController::AnimationCallback(float x) {
	FVector2D l_Size;
	GEngine->GameViewport->GetViewportSize(l_Size);
	float l_Width = l_Size.X;

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