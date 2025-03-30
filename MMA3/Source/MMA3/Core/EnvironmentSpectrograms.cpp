// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentSpectrograms.h"

AEnvironmentSpectrograms::AEnvironmentSpectrograms()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>l_Mesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/Shape_Cube.Shape_Cube'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance>l_Mat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/Mapping/M_BasicWhiteShape.M_BasicWhiteShape'"));

	m_CubeMesh = l_Mesh.Object;
	m_CubeMaterial = (UMaterial*)l_Mat.Object;

	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(m_RootComponent);

	for (int l_i = 0; l_i < m_Spectrograms.Num(); l_i++) {
		m_Spectrograms[l_i]->DestroyComponent();
	}

	m_Spectrograms.Empty();

	for (int l_i = 0; l_i < 32; l_i++) {
		FString l_String = "Spectrogram Object " + FString::SanitizeFloat(l_i);
		FName l_Name = FName(*l_String);
		UStaticMeshComponent* l_Component = CreateDefaultSubobject<UStaticMeshComponent>(l_Name);
		l_Component->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		l_Component->SetRelativeLocation(FVector(0, l_i * 200, 0));
		l_Component->SetStaticMesh(m_CubeMesh);
		l_Component->SetMaterial(0, m_CubeMaterial);
		m_Spectrograms.Add(l_Component);

	}
}

// Called when the game starts or when spawned
void AEnvironmentSpectrograms::BeginPlay()
{
	Super::BeginPlay();

	s_Instances.Add(this);
}

// Called every frame
void AEnvironmentSpectrograms::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnvironmentSpectrograms::OnConstruction(const FTransform& p_Transform) {

}

void AEnvironmentSpectrograms::SetCurrentFrequency(TArray<float> p_Values) {
	for (int l_i = 0; l_i < m_Spectrograms.Num(); l_i++) {
		if (p_Values.IsValidIndex(l_i) == false) continue;

		m_Spectrograms[l_i]->SetRelativeScale3D(FVector(1 + (p_Values[l_i]), 1, 1));
	}
}

TArray<AEnvironmentSpectrograms*> AEnvironmentSpectrograms::s_Instances = TArray<AEnvironmentSpectrograms*>();