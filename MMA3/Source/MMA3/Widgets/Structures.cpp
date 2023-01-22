// Fill out your copyright notice in the Description page of Project Settings.


#include "Structures.h"

Structures::Structures()
{

}

Structures::~Structures()
{

}

FMapInfo::FMapInfo() {

}

FMapInfo::FMapInfo(FString p_SongName, FString p_SongSubName, FString p_SongAuthor, FString p_SongMapper, FString p_MapPath, FString p_AudioFileName) {
	SongName = p_SongName;
	SongSubName = p_SongSubName;
	SongAuthor = p_SongAuthor;
	SongMapper = p_SongMapper;
	MapPath = p_MapPath;
	AudioFileName = p_AudioFileName;
}
