// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

// Sets default values
AC_Controller::AC_Controller()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AC_Controller::BeginPlay()
{
	Super::BeginPlay();

	/*try {
		m_MapSelectionWidget = CreateWidget<ULevelSelectionWidget>(GetWorld()->GetFirstPlayerController(), m_MapSelectionWidgetModel);
	}
	catch(std::exception) {
		GEngine->AddOnScreenDebugMessage(0, 10, FColor::Red, FString("Error happened"));
	}*/

}

// Called every frame
void AC_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerControllerReference == nullptr)
		PlayerControllerReference = GetWorld()->GetFirstPlayerController();

	if (CurrentScene == "MainMenu")
		PlayerControllerReference->bShowMouseCursor = true;
}

