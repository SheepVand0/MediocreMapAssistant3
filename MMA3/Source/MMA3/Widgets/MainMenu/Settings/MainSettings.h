// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MMA3/Widgets/Components/CustomEditableTextBox.h"
#include "MMA3/MMAConfig.h"
#include "MainSettings.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNeedBlueprintInit);

/**
 * 
 */
UCLASS()
class MMA3_API UMainSettings : public UUserWidget
{
	GENERATED_BODY()
	
protected: 

	virtual void NativeConstruct() override;

public:

	

	/////////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UCustomEditableTextBox* GamePath;

	/////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintImplementableEvent)
		void OnNeedBlueprintInitialization();

	UFUNCTION(BlueprintCallable)
		void LoadConfig();

	UFUNCTION(BlueprintCallable)
		void SaveConfig();

	/////////////////////////////////////////////////////////////////////////////

	UFUNCTION()
		void OnGamePathChanged(const FText& x);
};
