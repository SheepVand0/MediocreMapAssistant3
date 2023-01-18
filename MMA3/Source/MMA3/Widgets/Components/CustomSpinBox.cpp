// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSpinBox.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MMA3/Widgets/WidgetUtils.h"

UCustomSpinBox::UCustomSpinBox() {

	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_BackgroundMaterial(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_RoundedBackground.M_RoundedBackground'"));

}