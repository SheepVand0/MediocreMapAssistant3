// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Spacer.h"
#include "MMA3/Widgets/Enums.h"
#include "MMA3/Widgets/Structures.h"
#include "HAL/FileManagerGeneric.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "MMA3/Widgets/Components/MapList.h"
#include "MMA3/Widgets/Components/CustomButton.h"
#include "C:\UE\UE5_1_1\UE_5.1\Engine\Plugins\Marketplace\VaRestPlugin\Source\VaRest\Public\VaRestSubsystem.h"
#include "C:\UE\UE5_1_1\UE_5.1\Engine\Plugins\Marketplace\VaRestPlugin\Source\VaRest\Public\VaRestJsonObject.h"
#include "C:\UE\UE5_1_1\UE_5.1\Engine\Plugins\Marketplace\VaRestPlugin\Source\VaRest\Public\VaRestJsonValue.h"
#include "Components/VerticalBox.h"
#include "LevelSelectionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMapCellNeedToBeAdded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlueprintIsReady);

/**
 *
 */
UCLASS()
class MMA3_API ULevelSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	static ULevelSelectionWidget* Instance;

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintAssignable)
		FOnMapCellNeedToBeAdded OnMapCellNeedToBeAdded;

	UPROPERTY(BlueprintAssignable)
		FOnBlueprintIsReady OnBlueprintReady;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCanvasPanel* MainCanvas;

	/////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class USpacer* NoMapSelectedSpacer;

	/////////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UMapList* LevelsScrollBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UVerticalBox* MapDetailsBox;

	/////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomButton* EditButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomButton* DeleteButton;

	/////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* CoverPreview;

	/////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomTextBlock* SongName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomTextBlock* SongSubName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomTextBlock* SongAuthor;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomTextBlock* Mapper;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomTextBlock* SongDuration;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCustomTextBlock* SongBpm;

public:

	UPROPERTY()
		TArray<FMapInfo> m_Maps;

	UFUNCTION(BlueprintCallable)
		void EventBlueprintReady();

	UFUNCTION()
		void RefreshMaps(EMapListType p_Type);

	UFUNCTION()
		void OnMapSelected();

	UFUNCTION()
		void OnEditButtonPressed();

};

ULevelSelectionWidget* ULevelSelectionWidget::Instance = nullptr;

struct FDirectoryVisitor : public IPlatformFile::FDirectoryVisitor
{

	bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
	{

		if (bIsDirectory) {

			FString l_Directory = FilenameOrDirectory;
			l_Directory = l_Directory.Replace(TEXT("/"), TEXT("\\"));

			ULevelSelectionWidget* l_Widget = ULevelSelectionWidget::Instance;

			IPlatformFile& l_FileManager = FPlatformFileManager::Get().GetPlatformFile();

			if (!l_FileManager.FileExists(*(l_Directory + FString("\\info.dat")))) return true;

			FString l_FileResult;

			FFileHelper::LoadFileToString(l_FileResult, *(l_Directory + FString("\\info.dat")));

			FMapInfo l_Info = FMapInfo{};

			l_Info.FromJson(l_FileResult, l_Directory);

			l_Widget->m_Maps.Add(l_Info);
		}

		return true;
	}
};