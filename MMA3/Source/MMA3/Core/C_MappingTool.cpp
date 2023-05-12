// Fill out your copyright notice in the Description page of Project Settings.

#include "C_MappingTool.h"
#include <iostream>

// Sets default values
AC_MappingTool::AC_MappingTool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ToolMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tool Mesh"));
	ToolMeshComp->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AC_MappingTool::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AC_MappingTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UMappingSubTool* AC_MappingTool::GetSelectedSubTool() {
	return CurrentSubTool;
}

void AC_MappingTool::SelectSubTool(int p_Index) {
	if (!SubTools.IsValidIndex(p_Index)) return;

	CurrentSubTool = SubTools[p_Index];

	Update();
}

void AC_MappingTool::OnUse(FVector p_Position) {
	auto l_SubTool = GetSelectedSubTool();
	if (l_SubTool == nullptr) return;

	l_SubTool->OnUse(p_Position);
}

void AC_MappingTool::Update() {

	UMappingSubTool* l_MappingSubTool = GetSelectedSubTool();
	if (l_MappingSubTool == nullptr) return;

	UStaticMesh* l_ToolMesh = l_MappingSubTool->GetToolMesh();

	UMaterialInterface* l_ToolMaterial = l_MappingSubTool->GetToolMaterial();

	if (l_ToolMesh != nullptr)
		ToolMeshComp->SetStaticMesh(l_ToolMesh);

	if (l_ToolMaterial != nullptr)
		ToolMeshComp->SetMaterial(0, l_ToolMaterial);

	ToolMeshComp->SetRelativeLocation(l_MappingSubTool->GetCustomPosition());
	ToolMeshComp->SetRelativeRotation(l_MappingSubTool->GetCustomRotation());
	ToolMeshComp->SetRelativeScale3D(l_MappingSubTool->GetCustomScale());
}

UStaticMesh* UMappingSubTool::GetToolMesh() {
	return ToolMesh;
}

UMaterialInterface* UMappingSubTool::GetToolMaterial() {
	return ToolMaterial;
}

void UMappingSubTool::OnUse(FVector p_Position) {
	std::string l_ExceptionText = "OnUse method not overrided on tool";
	throw new std::exception(l_ExceptionText.c_str());
}

FVector UMappingSubTool::GetCustomPosition() {
	return CustomPosition;
}

FRotator UMappingSubTool::GetCustomRotation() {
	return CustomRotation;
}

FVector UMappingSubTool::GetCustomScale() {
	return CustomScale;
}