// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomComboString.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MMA3/Widgets/WidgetUtils.h"

UCustomComboString::UCustomComboString() {

	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_MaterialBackground(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_RoundedBackgroundLight.M_RoundedBackgroundLight'"));

	/////////////////////////////////////////////////////////////////////////////

	FSlateBrush l_Brush = UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_MaterialBackground.Object);
	l_Brush = WidgetUtils::GetUIElementStyle(l_Brush, 16, FColor(0, 0, 0), FMargin(0.5f));

	/////////////////////////////////////////////////////////////////////////////

	WidgetStyle.ComboButtonStyle.ButtonStyle.Normal = l_Brush;
	WidgetStyle.ComboButtonStyle.MenuBorderBrush = l_Brush;
	ItemStyle.ActiveBrush = l_Brush;

	/////////////////////////////////////////////////////////////////////////////

	/*ItemStyle.SelectedTextColor = FColor::White;
	FSlateFontInfo l_NewFont = FSlateFontInfo("Font'/Game/Assets/Fonts/Teko/F_Teko.F_Teko'", 20);*/
	ItemStyle.SelectedTextColor = FColor::White;
	ItemStyle.TextColor = FColor::White;

	/////////////////////////////////////////////////////////////////////////////

	ItemStyle.ActiveHoveredBrush.TintColor = FColor(20, 20, 20);
	ItemStyle.DropIndicator_Above.TintColor = FColor::White;
	ItemStyle.DropIndicator_Below.TintColor = FColor::White;
	ItemStyle.DropIndicator_Onto.TintColor = FColor::White;

	/////////////////////////////////////////////////////////////////////////////

	ForegroundColor = FColor::White;

	/////////////////////////////////////////////////////////////////////////////

	l_Brush.TintColor = FColor(70, 70, 70);
	WidgetStyle.ComboButtonStyle.ButtonStyle.Hovered = l_Brush;

	/////////////////////////////////////////////////////////////////////////////

	l_Brush.TintColor = FColor(0, 0, 0);
	WidgetStyle.ComboButtonStyle.ButtonStyle.Pressed = l_Brush;

	RenderTransform.Shear = FVector2D(-1, 0);
}