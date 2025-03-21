// Fill out your copyright notice in the Description page of Project Settings.


#include "FastAnimator.h"
#include "Kismet/GameplayStatics.h"

float FFastAnimation::Evaluate(float time, bool& finished) {

	int l_i = 0;
	while (l_i < Keys.Num() - 1) {
		FAnimationKey* l_Key = Keys[l_i];
		FAnimationKey* l_NextKey = Keys[l_i + 1];

		if (l_Key->Time <= time && l_NextKey->Time >= time) {

			float l_KeysTime = time - l_Key->Time;
			float l_Value = l_Key->Value + ((l_KeysTime / (l_NextKey->Time - l_Key->Time)) * (l_NextKey->Value - l_Key->Value));
			return l_Value;
		}
		l_i += 1;
	}
	//finished = true;
	return Keys.Last()->Value;
}

//////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// Sets default values
AFastAnimator::AFastAnimator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Instance = this;
}

static void Test(float value) {
	GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Red, FString::SanitizeFloat(value));
}

// Called when the game starts or when spawned
void AFastAnimator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFastAnimator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LastActorTime = ActorTime;
	ActorTime = UGameplayStatics::GetTimeSeconds(GetWorld());

	if (Animations.Num() == 0) return;

	for (int l_i = 0; l_i < Animations.Num(); l_i++) {
		ProcessAnimation(Animations[l_i], ActorTime);
	}

	for (int l_i = 0; l_i < AnimationsToRemove.Num(); l_i++) {
		Animations.Remove(AnimationsToRemove[l_i]);
	}
	if (AnimationsToRemove.Num() > 0)
		AnimationsToRemove.Empty();
}

//////////////////////////////////////////////////////////////////

void AFastAnimator::Play(FFastAnimation* animation) {
	if (animation == nullptr) return;

	if (animation->CallbackFunction == nullptr) return;

	animation->AnimationStartTime = ActorTime;

	Animations.Add(animation);
}

//////////////////////////////////////////////////////////////////

void AFastAnimator::ProcessAnimation(FFastAnimation* animation, float time) {
	FAnimationKey* CurrentKey = animation->CurrentKey;
	TArray<FAnimationKey*> Keys = animation->Keys;

	/*if (CurrentKey == nullptr) {
		animation->CurrentKey = Keys[0];
		if (Keys.Num() > 1)
			animation->NextKey = animation->Keys[1];
	}

	FastDebugMessage(3, FString::SanitizeFloat(time - animation->AnimationStartTime));

	if (animation->NextKey->Time <= time - animation->AnimationStartTime) {
		animation->CurrentKey = animation->NextKey;
		animation->CurrentKeyIndex += 1;

		if (animation->CurrentKeyIndex < animation->Keys.Num() - 1) {
			animation->NextKey = animation->Keys[animation->CurrentKeyIndex + 1];
		}
		else {
			AnimationsToRemove.Add(animation);
			return;
		}
	}

	float l_AnimationTime = time - animation->AnimationStartTime - animation->CurrentKey->Time;
	float l_CurrentValue = 0;
	float l_BaseValue = (animation->NextKey->Value - animation->CurrentKey->Value) * (l_AnimationTime / animation->AnimationDuration);
	float l_FixedValue = l_BaseValue;
	if (animation->CurrentKey->ExponentFunction != nullptr) {
		if (animation->CurrentKey->ExponentFunction->CanInvoke())
			l_FixedValue *= animation->CurrentKey->ExponentFunction->Invoke((l_AnimationTime / animation->AnimationDuration));
	}
	float l_FinalValue = animation->CurrentKey->Value + l_FixedValue;*/

	bool l_Finished;
	float l_Value = animation->Evaluate(time - animation->AnimationStartTime, l_Finished);
	FastDebugMessage(1, FString::SanitizeFloat(l_Value));

	if (l_Finished == true) {
		AnimationsToRemove.Add(animation);
		if (animation->FinishedFunction != nullptr) {
			if (animation->FinishedFunction->CanInvoke())
				animation->FinishedFunction->Invoke();
		}
	}

	if (animation->CallbackFunction == nullptr) return;

	if (!animation->CallbackFunction->CanInvoke()) return;

	animation->CallbackFunction->Invoke(l_Value);
}

