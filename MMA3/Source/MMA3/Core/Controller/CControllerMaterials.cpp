#include "CController.h"

UMaterialInstance* ACController::UpdateNoteMaterial(UMaterialInterface* noteMaterial, int type, bool passed)
{
	if (GetConfig() == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Config not defined!"));
		return nullptr;
	}

	FLinearColor l_LeftColor = GetConfig()->LeftEditorColor;
	FLinearColor l_RightColor = GetConfig()->RightEditorColor;

	UMaterialInstanceDynamic* l_NoteMaterialDynamic = UMaterialInstanceDynamic::Create(noteMaterial, nullptr);
	if (type == 0) {
		l_NoteMaterialDynamic->SetVectorParameterValue("NoteColor", l_LeftColor);
	}
	else if (type == 1) {
		l_NoteMaterialDynamic->SetVectorParameterValue("NoteColor", l_RightColor);
	}

	if (passed) {
		l_NoteMaterialDynamic->SetScalarParameterValue("OpacityMultiplier", .5f);
	}
	
	return l_NoteMaterialDynamic;
}