// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Spacer.h"
#include "MMA3/Widgets/Enums.h"
#include "MMA3/Widgets/Structures.h"
#include "HAL/FileManagerGeneric.h"
#include "VaRest/Public/VaRestJsonObject.h"
#include "VaRest/Public/VaRestJsonValue.h"
#include "LevelSelectionWidget.generated.h"

/**
 *
 */
UCLASS(Abstract)
class MMA3_API ULevelSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	static ULevelSelectionWidget* Instance;

	static IPlatformFile& s_FileManager;

	virtual void PostLoad() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCanvasPanel* MainCanvas;

	/////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class USpacer* NoMapSelectedSpacer;

	/////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UMapList* LevelsScrollBox;

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

	/////////////////////////////////////////////////////////////////////////////

public:

	UPROPERTY()
		TArray<FMapInfo> m_Maps;

	UFUNCTION()
		void RefreshMaps(EMapListType p_Type);

};


struct FDirectoryVisitor : public IPlatformFile::FDirectoryVisitor
{
	bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
	{

		ULevelSelectionWidget* l_Widget = ULevelSelectionWidget::Instance;

		IFileHandle* l_FileHandle = l_Widget->s_FileManager.OpenRead(FilenameOrDirectory);

		//l_Widget->m_Maps.Add();

		return true;
	}
};