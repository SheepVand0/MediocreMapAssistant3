// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSpinBox.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MMA3/Widgets/WidgetUtils.h"

UCustomSpinBox::UCustomSpinBox() {

	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_BackgroundMaterial(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_Slider_1.M_Slider_1'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_BackgroundMaterialHovered(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_Slider_1_Hovered.M_Slider_1_Hovered'"));

	/////////////////////////////////////////////////////////////////////////////

	FSlateBrush l_Background = UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_BackgroundMaterial.Object);
	l_Background = WidgetUtils::GetUIElementStyle(l_Background, 32.0f, FColor::White, FMargin(0.5f));

	FSlateBrush l_BackgroundHovered = UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_BackgroundMaterialHovered.Object);
	l_BackgroundHovered = WidgetUtils::GetUIElementStyle(l_BackgroundHovered, 32.0f, FColor::White, FMargin(0.5f));

	/////////////////////////////////////////////////////////////////////////////

	WidgetStyle.BackgroundBrush = l_Background;

	/////////////////////////////////////////////////////////////////////////////

	l_Background.TintColor = FLinearColor(5.0f, 5.0f, 5.0f);
	WidgetStyle.ActiveBackgroundBrush = l_Background;

	/////////////////////////////////////////////////////////////////////////////

	WidgetStyle.HoveredBackgroundBrush = l_BackgroundHovered;

	RenderTransform.Shear = FVector2D(-1, 0);

}