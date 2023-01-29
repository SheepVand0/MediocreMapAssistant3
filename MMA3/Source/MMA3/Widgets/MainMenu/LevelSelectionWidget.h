// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Spacer.h"
#include "MMA3/Widgets/Enums.h"
#include "MMA3/Widgets/Structures.h"
#include "HAL/FileManagerGeneric.h"
#include "MMA3/Widgets/Components/MapList.h"
#include "C:\Program Files\Epic Games\UE_5.1\Engine\Plugins\Marketplace\VaRestPlugin\Source\VaRest\Public\VaRestSubsystem.h"
#include "C:\Program Files\Epic Games\UE_5.1\Engine\Plugins\Marketplace\VaRestPlugin\Source\VaRest\Public\VaRestJsonObject.h"
#include "C:\Program Files\Epic Games\UE_5.1\Engine\Plugins\Marketplace\VaRestPlugin\Source\VaRest\Public\VaRestJsonValue.h"
#include "Components/VerticalBox.h"
#include "LevelSelectionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMapCellNeedToBeAdded)

/**
 *
 */
UCLASS()
class MMA3_API ULevelSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	static ULevelSelectionWidget* Instance;

	virtual void PostLoad() override;

	UPROPERTY(BlueprintAssignable)
		FOnMapCellNeedToBeAdded OnMapCellNeedToBeAdded;

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

	UFUNCTION()
		void RefreshMaps(EMapListType p_Type);

	UFUNCTION()
		void OnMapSelected();



};

ULevelSelectionWidget* ULevelSelectionWidget::Instance = nullptr;

struct FDirectoryVisitor : public IPlatformFile::FDirectoryVisitor
{
	bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
	{

		GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor::White, FString(FilenameOrDirectory));

		if (!bIsDirectory) return false;

		ULevelSelectionWidget* l_Widget = ULevelSelectionWidget::Instance;

		IPlatformFile& l_FileManager = FPlatformFileManager::Get().GetPlatformFile();

		if (!l_FileManager.FileExists(*(FilenameOrDirectory + FString("\\info.dat")))) return true;

		IFileHandle* l_FileHandle = l_FileManager.OpenRead(*(FilenameOrDirectory + FString("\\info.dat")));

		FString l_FileResult;

		FFileHelper::LoadFileToString(l_FileResult, *(FilenameOrDirectory + FString("\\info.dat")));

		FMapInfo l_Info = FMapInfo{};

		l_Info.FromJson(l_FileResult, FilenameOrDirectory);

		l_Widget->m_Maps.Add(l_Info);

		return true;
	}
};