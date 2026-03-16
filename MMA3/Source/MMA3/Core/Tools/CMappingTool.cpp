// Fill out your copyright notice in the Description page of Project Settings.

#include "CMappingTool.h"
#include <iostream>

// Sets default values
ACMappingTool::ACMappingTool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ToolMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tool Mesh"));
	ToolMeshComp->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ACMappingTool::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACMappingTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void ACMappingTool::Update() {
	UE_LOG(LogTemp, Display, TEXT("I'm the tool and I'm updating"));

	UStaticMesh* l_ToolMesh = GetToolMesh();

	UMaterialInterface* l_ToolMaterial = GetToolMaterial();

	ToolMeshComp->SetStaticMesh(l_ToolMesh);

	ToolMeshComp->SetMaterial(0, l_ToolMaterial);

	ToolMeshComp->SetRelativeLocation(GetCustomPosition());
	ToolMeshComp->SetRelativeRotation(GetCustomRotation());
	ToolMeshComp->SetRelativeScale3D(GetCustomScale());
}

UStaticMesh* ACMappingTool::GetToolMesh() {
	return ToolMesh;
}

UMaterialInterface* ACMappingTool::GetToolMaterial() {
	return ToolMaterial;
}

void ACMappingTool::OnUse(FVector p_Position) {
	std::string l_ExceptionText = "OnUse method not overriden on tool";
	throw new std::exception(l_ExceptionText.c_str());
}

FVector ACMappingTool::GetCustomPosition() {
	return CustomPosition;
}

FRotator ACMappingTool::GetCustomRotation() {
	return CustomRotation;
}

FVector ACMappingTool::GetCustomScale() {
	return CustomScale;
}