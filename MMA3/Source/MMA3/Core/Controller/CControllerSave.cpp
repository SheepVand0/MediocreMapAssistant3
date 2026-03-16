#include "CController.h"

void ACController::Save()
{
	if (!MapContent) return;

	SortMapContent();

	UE_LOG(LogTemp, Warning, TEXT("Saving map"));
	FString l_Path = FString(MapInfo->MapPath + "\\" + Difficulty->_beatmapFilename);
	UE_LOG(LogTemp, Warning, TEXT("Map path: %s"), *l_Path);

	UVaRestJsonObject* l_MapJson = MapContent->ToJson();
	FFileHelper::SaveStringToFile(l_MapJson->EncodeJson(), *l_Path, FFileHelper::EEncodingOptions::ForceUTF8);
}
