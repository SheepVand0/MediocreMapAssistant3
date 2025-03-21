// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMA3/Misc/SActionPtr.h"
#include "FastAnimator.generated.h"

#define FastDebugMessage(Index, Text) GEngine->AddOnScreenDebugMessage(Index, 5.f, FColor::Blue, Text);

class FAnimationKey {

public:

	FAnimationKey() {
		Time = 0;
		Value = 0;
	}

	FAnimationKey(float time, float value, SActionPtrBase<float, float>* exponentFunction = nullptr) {
		Time = time;
		Value = value;
		ExponentFunction = exponentFunction;
	}

	UPROPERTY(BlueprintReadWrite) float Time;

	UPROPERTY(BlueprintReadWrite) float Value;

	SActionPtrBase<float, float>* ExponentFunction = nullptr;

	bool operator < (FAnimationKey& x) {
		return Time > x.Time;
	}
};

class FFastAnimation {

public:

	FFastAnimation() {

	}

	FFastAnimation(FString animationId, TArray<FAnimationKey*> keys, SActionPtrBase<void, float>* callbackFunction, SActionPtrBase<void>* finishedFunction = nullptr) {
		AnimationId = animationId;
		Keys = keys;
		Keys.Sort();
		AnimationDuration = Keys.Last()->Time - Keys[0]->Time;
		CallbackFunction = callbackFunction;
		FinishedFunction = finishedFunction;
	}

	UPROPERTY(BlueprintReadWrite) FString AnimationId;
	TArray<FAnimationKey*> Keys;
	UPROPERTY(BlueprintReadOnly) float AnimationStartTime;
	UPROPERTY(BlueprintReadOnly) float AnimationDuration;

	SActionPtrBase<void, float>* CallbackFunction = nullptr;
	SActionPtrBase<void>* FinishedFunction = nullptr;

	UPROPERTY(BlueprintReadOnly) FAnimationKey* NextKey = nullptr;
	UPROPERTY(BlueprintReadOnly) FAnimationKey* CurrentKey = nullptr;
	UPROPERTY(BlueprintReadOnly) int CurrentKeyIndex = 0;

	UFUNCTION(BlueprintCallable) float Evaluate(float time, bool& finished);
};

UCLASS()
class MMA3_API AFastAnimator : public AActor
{

	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFastAnimator();

	static AFastAnimator* Instance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY() float LastActorTime = 0;
	UPROPERTY() float ActorTime = 0;
	UPROPERTY() USceneComponent* Root;
	TArray<FFastAnimation*> Animations;
	TArray<FFastAnimation*> AnimationsToRemove;

	void ProcessAnimation(FFastAnimation* Animation, float time);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) FORCEINLINE float GetTime() const { return ActorTime; }
	UFUNCTION(BlueprintCallable) FORCEINLINE float GetLastTime() const { return LastActorTime; }
	void Play(FFastAnimation* animation);
};

AFastAnimator* AFastAnimator::Instance = nullptr;