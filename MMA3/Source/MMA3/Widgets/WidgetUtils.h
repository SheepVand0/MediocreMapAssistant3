// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ContentWidget.h"

/**
 *
 */
class MMA3_API WidgetUtils
{
public:
	WidgetUtils();
	~WidgetUtils();

	static FSlateBrush GetUIElementStyle(FSlateBrush p_SlateBrush, float p_Size, FColor p_Color, FMargin p_Margin) {
		p_SlateBrush.SetImageSize(FVector2D(p_Size, p_Size));
		p_SlateBrush.DrawAs = ESlateBrushDrawType::Box;
		p_SlateBrush.TintColor = p_Color;
		p_SlateBrush.Margin = p_Margin;
		return p_SlateBrush;
	}

};

template<typename T, typename F>
void foreach(T &p_Container, F p_Func) {
	for (T l_Item : p_Container) {
		p_Func(l_Item);
	}
};