// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSpinBox.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MMA3/Widgets/WidgetUtils.h"

UCustomSpinBox::UCustomSpinBox() {

	ConstructorHelpers::FObjectFinder<UMaterial> l_BackgroundMaterial(TEXT("/Script/Engine.Material'/Game/Assets/Materials/MBlackTranslucent.MBlackTranslucent'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_BackgroundMaterialHovered(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/MHovered.MHovered'"));

	/////////////////////////////////////////////////////////////////////////////

	FSlateBrush l_Background = UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_BackgroundMaterial.Object);
	l_Background = WidgetUtils::GetUIElementStyle(l_Background, 32.0f, FColor::White, FMargin(0.5f));

	FSlateBrush l_BackgroundHovered = UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_BackgroundMaterialHovered.Object);
	l_BackgroundHovered = WidgetUtils::GetUIElementStyle(l_BackgroundHovered, 32.0f, FColor::White, FMargin(0.5f));

	/////////////////////////////////////////////////////////////////////////////

	WidgetStyle.BackgroundBrush = l_Background;

	/////////////////////////////////////////////////////////////////////////////

	l_Background.TintColor = FLinearColor(0.f, 1.f, 1.f);
	WidgetStyle.ActiveBackgroundBrush = l_Background;

	/////////////////////////////////////////////////////////////////////////////

	WidgetStyle.HoveredBackgroundBrush = l_BackgroundHovered;

	/////////////////////////////////////////////////////////////////////////////

	WidgetStyle.ForegroundColor = FSlateColor(FLinearColor(0, 1, 1));

	/////////////////////////////////////////////////////////////////////////////

	RenderTransform.Shear = FVector2D(-1, 0);

	Font.Size = 9;
}

TSharedPtr<SSpinBox<float>> UCustomSpinBox::GetSpinBox()
{
	return MySpinBox;
}
