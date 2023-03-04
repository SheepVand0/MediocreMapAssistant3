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

	if (GetSelectedSubTool() != nullptr) {
		if (GetSelectedSubTool()->GetToolMesh() != nullptr)
			ToolMeshComp->SetStaticMesh(GetSelectedSubTool()->GetToolMesh());
	}
}

// Called every frame
void AC_MappingTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UMappingSubTool* AC_MappingTool::GetSelectedSubTool() {
	if (SubTools.Num() == 0)
		return nullptr;

	if (!SubTools.IsValidIndex(SelectedSubToolIndex)) return nullptr;

	return SubTools[SelectedSubToolIndex];
}

void AC_MappingTool::OnUse(FVector p_Position) {
	auto l_SubTool = GetSelectedSubTool();
	if (l_SubTool == nullptr) return;

	l_SubTool->OnUse(p_Position);
}

UStaticMesh* UMappingSubTool::GetToolMesh() {
	return ToolMesh;
}

void UMappingSubTool::OnUse(FVector p_Position) {
	std::string l_ExceptionText = "OnUse method not overrided on tool";
	throw new std::exception(l_ExceptionText.c_str());
}