// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomEditableTextBox.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MMA3/Widgets/WidgetUtils.h"

UCustomEditableTextBox::UCustomEditableTextBox() {

	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_BackgroundMaterial(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_RoundedBackgroundLight.M_RoundedBackgroundLight'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_FocusedMaterial(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_Slider_1_Hovered.M_Slider_1_Hovered'"));

	/////////////////////////////////////////////////////////////////////////////

	FSlateBrush l_Brush = UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_BackgroundMaterial.Object);
	l_Brush = WidgetUtils::GetUIElementStyle(l_Brush, 16, FColor::Black, FMargin(0.5f));

	FSlateBrush l_Focused = UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_FocusedMaterial.Object);
	l_Focused = WidgetUtils::GetUIElementStyle(l_Focused, 16, FColor::White, FMargin(0.5f));

	/////////////////////////////////////////////////////////////////////////////

	WidgetStyle.BackgroundImageNormal = l_Brush;
	WidgetStyle.BackgroundImageReadOnly = l_Brush;

	/////////////////////////////////////////////////////////////////////////////

	l_Brush.TintColor = FColor(150, 150, 150);

	WidgetStyle.BackgroundImageHovered = l_Brush;
	WidgetStyle.BackgroundImageFocused = l_Brush;

	/////////////////////////////////////////////////////////////////////////////

	RenderTransform.Shear = FVector2D(-1, 0);
}