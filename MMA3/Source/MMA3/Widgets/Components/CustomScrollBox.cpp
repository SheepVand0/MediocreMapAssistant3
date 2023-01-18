// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomScrollBox.h"
#include "UObject/ConstructorHelpers.h"
#include "MMA3/Widgets/WidgetUtils.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UCustomScrollBox::UCustomScrollBox() {

	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_ScrollBarMaterial(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_RoundedBackgroundLight.M_RoundedBackgroundLight'"));

	/////////////////////////////////////////////////////////////////////////////

	FSlateBrush l_BackgroundBrush = WidgetUtils::GetUIElementStyle(UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_ScrollBarMaterial.Object), 16.0f, FColor::Black, FMargin(0.5f));

	/////////////////////////////////////////////////////////////////////////////

	WidgetBarStyle.SetHorizontalBackgroundImage(l_BackgroundBrush);
	WidgetBarStyle.SetVerticalBackgroundImage(l_BackgroundBrush);

	/////////////////////////////////////////////////////////////////////////////

	l_BackgroundBrush.ImageSize = FVector2d(8.0f);

	l_BackgroundBrush.TintColor = FColor(80, 80, 80, 255/2);
	WidgetBarStyle.SetNormalThumbImage(l_BackgroundBrush);

	/////////////////////////////////////////////////////////////////////////////

	l_BackgroundBrush.TintColor = FColor(220, 220, 220, 255/2);
	WidgetBarStyle.SetHoveredThumbImage(l_BackgroundBrush);

	/////////////////////////////////////////////////////////////////////////////

	l_BackgroundBrush.TintColor = FColor(120, 120, 120, 255/2);
	WidgetBarStyle.SetDraggedThumbImage(l_BackgroundBrush);

}