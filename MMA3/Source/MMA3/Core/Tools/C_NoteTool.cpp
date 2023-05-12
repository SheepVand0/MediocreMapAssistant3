// Fill out your copyright notice in the Description page of Project Settings.


#include "C_NoteTool.h"
#include <Kismet/GameplayStatics.h>
#include "MMA3/MMAConfig.h"

AC_NoteTool::AC_NoteTool() {
	USubNoteTool* l_NoteTool = CreateDefaultSubobject<USubNoteTool>(TEXT("Sub note tool"));
	l_NoteTool->SetupAttachment(GetRootComponent());
	SubTools.Add(l_NoteTool);
}

void AC_NoteTool::OnUse(FVector p_Position) {

}

USubNoteTool::USubNoteTool() {

	ConstructorHelpers::FObjectFinder<UStaticMesh> l_ToolMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/NoteBody_Cube.NoteBody_Cube'"));
	ConstructorHelpers::FObjectFinder<UMaterial> l_ToolMaterial(TEXT("/Script/Engine.Material'/Game/Assets/Materials/Mapping/M_Note.M_Note'"));

	CustomScale = FVector(-0.08f, -0.08f, -0.08f);

	NoteToolMesh = l_ToolMesh.Object;
	NoteMaterial = l_ToolMaterial.Object;

	Type = 1;
}

///////////////////////////////////////////////////////////

void USubNoteTool::OnUse(FVector p_Position) {

}

UStaticMesh* USubNoteTool::GetToolMesh() {
	if (Type == 0)
		return BombToolMesh;
	else
		return NoteToolMesh;
}

UMaterialInterface* USubNoteTool::GetToolMaterial() {
	if (Type == 0)
		return BombMaterial;
	else {
		if (UGameplayStatics::DoesSaveGameExist(MMA_SAVE_GAME_SLOT_NAME, 0) && Type != 3) {
			UMMAConfig* l_Config = Cast<UMMAConfig>(UGameplayStatics::LoadGameFromSlot(MMA_SAVE_GAME_SLOT_NAME, 0));

			UMaterialInstanceDynamic* l_Dynamic = UMaterialInstanceDynamic::Create(NoteMaterial, this);
			if (Type == 1) {
				l_Dynamic->SetVectorParameterValue(FName("NoteColor"), FVector4(l_Config->LeftEditorColor.R, l_Config->LeftEditorColor.G, l_Config->LeftEditorColor.B, 1));
			}
			else if (Type == 2) {
				l_Dynamic->SetVectorParameterValue(FName("NoteColor"), FVector4(l_Config->RightEditorColor.R, l_Config->RightEditorColor.G, l_Config->RightEditorColor.B, 1));
			}
			return l_Dynamic;
		}
		return NoteMaterial;
	}
}