// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnvironmentSpectrograms.generated.h"

UCLASS()
class MMA3_API AEnvironmentSpectrograms : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnvironmentSpectrograms();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& p_Transform) override;

	UStaticMesh* m_CubeMesh;

	UMaterial* m_CubeMaterial;

	USceneComponent* m_RootComponent;

	TArray<UStaticMeshComponent*> m_Spectrograms;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetCurrentFrequency(TArray<float> p_Values);

	static TArray<AEnvironmentSpectrograms*> s_Instances;
};
