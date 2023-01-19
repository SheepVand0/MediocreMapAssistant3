// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomButton.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MMA3/Widgets/WidgetUtils.h"

UCustomButton::UCustomButton() {
	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_Material(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_Button.M_Button'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_Hovered(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_ButtonHover.M_ButtonHover'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_Pressed(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_ButtonPressed.M_ButtonPressed'"));

	/////////////////////////////////////////////////////////////////////////////

	FSlateBrush l_Brush = UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_Material.Object);
	WidgetStyle.SetNormal(WidgetUtils::GetUIElementStyle(l_Brush, 32.0f, FColor::White, FMargin(0.5f)));

	/////////////////////////////////////////////////////////////////////////////

	l_Brush = UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_Hovered.Object);
	WidgetStyle.SetHovered(WidgetUtils::GetUIElementStyle(l_Brush, 32.0f, FColor::White, FMargin(0.5f)));

	/////////////////////////////////////////////////////////////////////////////

	l_Brush = UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_Pressed.Object);
	l_Brush = WidgetUtils::GetUIElementStyle(l_Brush, 32.0f, FColor::White, FMargin(0.5f));

	WidgetStyle.SetPressed(l_Brush);
	WidgetStyle.SetDisabled(l_Brush);

	RenderTransform.Shear = FVector2D(-1, 0);
}