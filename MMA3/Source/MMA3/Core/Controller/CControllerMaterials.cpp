#include "CController.h"

UMaterialInstance* ACController::UpdateNoteMaterial(UMaterialInterface* noteMaterial, int type, bool passed, bool isSelected)
{
	if (GetConfig() == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Config not defined!"));
		return nullptr;
	}

	FLinearColor l_LeftColor = isSelected ? FLinearColor(1, 0.7f, 0.f) : GetConfig()->LeftEditorColor;
	FLinearColor l_RightColor = isSelected ? FLinearColor(0, 1, 0.5f) : GetConfig()->RightEditorColor;

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