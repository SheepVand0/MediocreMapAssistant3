// Fill out your copyright notice in the Description page of Project Settings.

#include "CMappingTool.h"
#include <iostream>

// Sets default values
ACMappingTool::ACMappingTool() : bInstantUse(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ToolMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tool Mesh"));
	ToolMeshComp->SetupAttachment(GetRootComponent());
	
	CustomScale = FVector::One();
}

// Called when the game starts or when spawned
void ACMappingTool::BeginPlay()
{	
	Super::BeginPlay();
}

void ACMappingTool::OnUpdateUse(const FVector& position)
{
	
}

void ACMappingTool::OnUpdate(const FVector& position)
{
	
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

void ACMappingTool::OnFinishUsing(FVector p_Position) {
	//std::string l_ExceptionText = "OnUse method not overriden on tool";
	//throw new std::exception(l_ExceptionText.c_str());
}

void ACMappingTool::OnStartUsing(FVector position)
{
}

void ACMappingTool::StartUsing(const FVector& position)
{
	if (bIsUsing)
	{
		bIsUsing = false;
		OnFinishUsing(position);
		return;
	}
	
	if (bInstantUse)
	{
		OnFinishUsing(position);
	} else
	{
		bIsUsing = true;
		OnStartUsing(position);	
	}
}

void ACMappingTool::UpdateUse(FVector position)
{
	if (!bIsUsing)
	{
		OnUpdate(position);
	} else
	{
		OnUpdateUse(position);
	}
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

bool ACMappingTool::IsUsing()
{
	return bIsUsing;
}
