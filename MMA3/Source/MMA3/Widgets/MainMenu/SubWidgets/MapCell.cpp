// Fill out your copyright notice in the Description page of Project Settings.


#include "MapCell.h"

void UMapCell::SetData(FMapInfo p_Info) {
	m_Info = p_Info;
}

FMapInfo UMapCell::GetData() {
	return FMapInfo(m_Info.SongName, m_Info.SongAuthor, m_Info.SongMapper, m_Info.MapPath, m_Info.AudioFileName);
}