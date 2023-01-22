// Fill out your copyright notice in the Description page of Project Settings.


#include "MapButton.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MMA3/Widgets/WidgetUtils.h"

UMapButton::UMapButton() {

	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_Background(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_MapButtonBackgroundGradient.M_MapButtonBackgroundGradient'"));

	/////////////////////////////////////////////////////////////////////////////

	FSlateBrush l_Brush = UWidgetBlueprintLibrary::MakeBrushFromMaterial(l_Background.Object);
	l_Brush = WidgetUtils::GetUIElementStyle(l_Brush, 500, FColor::White, FMargin(0));
	l_Brush.DrawAs = ESlateBrushDrawType::RoundedBox;
	l_Brush.TintColor = FLinearColor(0, 0.7529412f, 1);

	m_PressedBrush = l_Brush;
	/////////////////////////////////////////////////////////////////////////////

	l_Brush.TintColor = FLinearColor(0, 0, 0, 0);
	m_NormalBrush = l_Brush;

	/////////////////////////////////////////////////////////////////////////////

	l_Brush.TintColor = FLinearColor(1, 1, 1, 0.5f);

	WidgetStyle.Normal = m_NormalBrush;
	WidgetStyle.Hovered = l_Brush;
	WidgetStyle.Pressed = m_PressedBrush;

	/////////////////////////////////////////////////////////////////////////////

	OnClicked.AddDynamic(this, &UMapButton::SetSelected);

	//m_ListReference = p_ListReference;

	if (m_ListReference != nullptr)
		m_ListReference->OnMapButtonPressedEvent.AddDynamic(this, &UMapButton::SetUnselected);
}

void UMapButton::SetSelected() {
	if (m_ListReference != nullptr)
		m_ListReference->OnMapButtonPressedEvent.Broadcast();
	WidgetStyle.Normal = m_PressedBrush;

}

void UMapButton::SetUnselected() {
	WidgetStyle.Normal = m_NormalBrush;
}