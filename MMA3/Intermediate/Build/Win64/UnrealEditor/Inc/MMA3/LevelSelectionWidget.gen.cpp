// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MMA3/Widgets/MainMenu/LevelSelectionWidget.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeLevelSelectionWidget() {}
// Cross Module References
	MMA3_API UClass* Z_Construct_UClass_ULevelSelectionWidget_NoRegister();
	MMA3_API UClass* Z_Construct_UClass_ULevelSelectionWidget();
	UMG_API UClass* Z_Construct_UClass_UUserWidget();
	UPackage* Z_Construct_UPackage__Script_MMA3();
	UMG_API UClass* Z_Construct_UClass_UCanvasPanel_NoRegister();
// End Cross Module References
	void ULevelSelectionWidget::StaticRegisterNativesULevelSelectionWidget()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ULevelSelectionWidget);
	UClass* Z_Construct_UClass_ULevelSelectionWidget_NoRegister()
	{
		return ULevelSelectionWidget::StaticClass();
	}
	struct Z_Construct_UClass_ULevelSelectionWidget_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MainCanvas_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_MainCanvas;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ULevelSelectionWidget_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UUserWidget,
		(UObject* (*)())Z_Construct_UPackage__Script_MMA3,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULevelSelectionWidget_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n *\n */" },
		{ "IncludePath", "Widgets/MainMenu/LevelSelectionWidget.h" },
		{ "ModuleRelativePath", "Widgets/MainMenu/LevelSelectionWidget.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULevelSelectionWidget_Statics::NewProp_MainCanvas_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "LevelSelectionWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Widgets/MainMenu/LevelSelectionWidget.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ULevelSelectionWidget_Statics::NewProp_MainCanvas = { "MainCanvas", nullptr, (EPropertyFlags)0x0010000000080009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULevelSelectionWidget, MainCanvas), Z_Construct_UClass_UCanvasPanel_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ULevelSelectionWidget_Statics::NewProp_MainCanvas_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULevelSelectionWidget_Statics::NewProp_MainCanvas_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ULevelSelectionWidget_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULevelSelectionWidget_Statics::NewProp_MainCanvas,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ULevelSelectionWidget_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ULevelSelectionWidget>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ULevelSelectionWidget_Statics::ClassParams = {
		&ULevelSelectionWidget::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_ULevelSelectionWidget_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_ULevelSelectionWidget_Statics::PropPointers),
		0,
		0x00B010A1u,
		METADATA_PARAMS(Z_Construct_UClass_ULevelSelectionWidget_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ULevelSelectionWidget_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ULevelSelectionWidget()
	{
		if (!Z_Registration_Info_UClass_ULevelSelectionWidget.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ULevelSelectionWidget.OuterSingleton, Z_Construct_UClass_ULevelSelectionWidget_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ULevelSelectionWidget.OuterSingleton;
	}
	template<> MMA3_API UClass* StaticClass<ULevelSelectionWidget>()
	{
		return ULevelSelectionWidget::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ULevelSelectionWidget);
	struct Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_MainMenu_LevelSelectionWidget_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_MainMenu_LevelSelectionWidget_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ULevelSelectionWidget, ULevelSelectionWidget::StaticClass, TEXT("ULevelSelectionWidget"), &Z_Registration_Info_UClass_ULevelSelectionWidget, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ULevelSelectionWidget), 1489462193U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_MainMenu_LevelSelectionWidget_h_1171922263(TEXT("/Script/MMA3"),
		Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_MainMenu_LevelSelectionWidget_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_MainMenu_LevelSelectionWidget_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
