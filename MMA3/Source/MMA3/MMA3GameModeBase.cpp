// Copyright Epic Games, Inc. All Rights Reserved.


#include "MMA3GameModeBase.h"
#include "Core/C_MapperPawn.h"

AMMA3GameModeBase::AMMA3GameModeBase() {

	ConstructorHelpers::FClassFinder<AC_MapperPawn> l_Class(TEXT("/Script/CoreUObject.Class'/Script/MMA3.C_MapperPawn'"));

	DefaultPawnClass = l_Class.Class;

}