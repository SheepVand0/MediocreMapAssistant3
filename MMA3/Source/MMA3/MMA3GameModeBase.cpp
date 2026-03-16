// Copyright Epic Games, Inc. All Rights Reserved.


#include "MMA3GameModeBase.h"
#include "Core/MapperPawn/CMapperPawn.h"

AMMA3GameModeBase::AMMA3GameModeBase() {

	//ConstructorHelpers::FClassFinder<ACMapperPawn> l_Class(TEXT("/Script/CoreUObject.Class'/Script/MMA3.CMapperPawn'"));

	DefaultPawnClass = ACMapperPawn::StaticClass();

}