// Fill out your copyright notice in the Description page of Project Settings.


#include "MapList.h"

void UMapCell::SetData(FMapInfo p_Info) {
	m_Info = p_Info;

	CName->SetText(FText::FromString(m_Info.SongName));
	CSubName->SetText(FText::FromString(m_Info.SongSubName));
	CAuthor->SetText(FText::FromString(m_Info.SongAuthor));
	CMapper->SetText(FText::FromString(m_Info.SongMapper));
	CBpm->SetText(FText::AsNumber(m_Info.BPM));

	CButton->OnClicked.AddDynamic(this, &UMapCell::SetSelected);

	m_ListReference->OnMapButtonPressedEvent.AddDynamic(this, &UMapCell::SetUnselected);
}

void UMapCell::SetSelected() {
	GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::White, FString("Pressed"));
	m_ListReference->SelectedCell = this;
	m_ListReference->OnMapButtonPressedEvent.Broadcast();
	CButton->WidgetStyle.SetNormal(CButton->m_PressedBrush);
}

void UMapCell::SetUnselected() {

	CButton->WidgetStyle.SetNormal(CButton->m_NormalBrush);

}

FMapInfo UMapCell::GetData() {
	return m_Info;
}