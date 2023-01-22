// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomBackground.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MMA3/Widgets/WidgetUtils.h"

UCustomBackground::UCustomBackground() {

	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_Background(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_RoundedBackground.M_RoundedBackground'"));

	/////////////////////////////////////////////////////////////////////////////

	FSlateBrush l_Brush = UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_Background.Object);
	l_Brush = WidgetUtils::GetUIElementStyle(l_Brush, 64, FColor::White, FMargin(0.5f));

	SetBrush(l_Brush);

}