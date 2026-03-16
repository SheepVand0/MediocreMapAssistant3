// Fill out your copyright notice in the Description page of Project Settings.


#include "MapList.h"

void UMapCell::SetData(FMapInfo* p_Info) {
	m_Info = p_Info;

	CName->SetText(FText::FromString(m_Info->_songName));
	CSubName->SetText(FText::FromString(m_Info->_songSubName));
	CAuthor->SetText(FText::FromString(m_Info->_songAuthorName));
	CMapper->SetText(FText::FromString(m_Info->_levelAuthorName));
	CBpm->SetText(FText::AsNumber(m_Info->_beatsPerMinute));

	CButton->OnClicked.AddDynamic(this, &UMapCell::SetSelected);

	m_ListReference->OnMapButtonPressedEvent.AddDynamic(this, &UMapCell::SetUnselected);
}

void UMapCell::SetSelected() {
	m_ListReference->SelectedCell = this;
	m_ListReference->OnMapButtonPressedEvent.Broadcast();
	CButton->WidgetStyle.SetNormal(CButton->m_PressedBrush);
}

void UMapCell::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UMapCell::SetUnselected() {

	CButton->WidgetStyle.SetNormal(CButton->m_NormalBrush);

}

FMapInfo* UMapCell::GetData() {
	return m_Info;
}