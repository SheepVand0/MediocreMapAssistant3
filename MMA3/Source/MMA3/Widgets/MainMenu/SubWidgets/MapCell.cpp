// Fill out your copyright notice in the Description page of Project Settings.


#include "MapCell.h"

void UMapCell::SetData(FMapInfo p_Info) {
	m_Info = p_Info;

}

FMapInfo UMapCell::GetData() {
	return m_Info;
}