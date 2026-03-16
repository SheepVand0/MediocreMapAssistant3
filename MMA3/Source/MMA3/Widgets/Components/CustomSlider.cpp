// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSlider.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MMA3/Widgets/WidgetUtils.h"

UCustomSlider::UCustomSlider() {

	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_SliderBackground(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_Slider_1.M_Slider_1'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_SliderHovered(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_Slider_1_Hovered.M_Slider_1_Hovered'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_SliderThumb(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_RoundedSliderBar.M_RoundedSliderBar'"));

	/////////////////////////////////////////////////////////////////////////////

	FSlateBrush l_Background = UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_SliderBackground.Object);
	l_Background = WidgetUtils::GetUIElementStyle(l_Background, 16, FColor::White, FMargin(0.5f));

	FSlateBrush l_Hovered = UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_SliderHovered.Object);
	l_Hovered = WidgetUtils::GetUIElementStyle(l_Hovered, 16, FColor::White, FMargin(0.5f));

	FSlateBrush l_Thumb = UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_SliderThumb.Object);
	l_Thumb = WidgetUtils::GetUIElementStyle(l_Thumb, 16, FColor::White, FMargin(0.5f));
	l_Thumb.ImageSize = FVector2d(4, 16);
	SetIndentHandle(true);

	/////////////////////////////////////////////////////////////////////////////

	WidgetStyle.SetNormalBarImage(l_Background);
	l_Background.TintColor = FColor::Black;
	WidgetStyle.SetDisabledBarImage(l_Background);
	WidgetStyle.SetHoveredBarImage(l_Hovered);

	/////////////////////////////////////////////////////////////////////////////

	WidgetStyle.SetNormalThumbImage(l_Thumb);
	WidgetStyle.SetHoveredThumbImage(l_Thumb);
	WidgetStyle.SetDisabledThumbImage(l_Thumb);

	WidgetStyle.SetBarThickness(30);

	RenderTransform.Shear = FVector2D(-1, 0);
}