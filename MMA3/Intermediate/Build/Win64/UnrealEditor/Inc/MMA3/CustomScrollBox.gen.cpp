// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MMA3/Widgets/Components/CustomScrollBox.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCustomScrollBox() {}
// Cross Module References
	MMA3_API UClass* Z_Construct_UClass_UCustomScrollBox_NoRegister();
	MMA3_API UClass* Z_Construct_UClass_UCustomScrollBox();
	UMG_API UClass* Z_Construct_UClass_UScrollBox();
	UPackage* Z_Construct_UPackage__Script_MMA3();
// End Cross Module References
	void UCustomScrollBox::StaticRegisterNativesUCustomScrollBox()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCustomScrollBox);
	UClass* Z_Construct_UClass_UCustomScrollBox_NoRegister()
	{
		return UCustomScrollBox::StaticClass();
	}
	struct Z_Construct_UClass_UCustomScrollBox_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UCustomScrollBox_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UScrollBox,
		(UObject* (*)())Z_Construct_UPackage__Script_MMA3,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCustomScrollBox_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n *\n */" },
		{ "IncludePath", "Widgets/Components/CustomScrollBox.h" },
		{ "ModuleRelativePath", "Widgets/Components/CustomScrollBox.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UCustomScrollBox_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCustomScrollBox>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UCustomScrollBox_Statics::ClassParams = {
		&UCustomScrollBox::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x00B000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UCustomScrollBox_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UCustomScrollBox_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UCustomScrollBox()
	{
		if (!Z_Registration_Info_UClass_UCustomScrollBox.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCustomScrollBox.OuterSingleton, Z_Construct_UClass_UCustomScrollBox_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UCustomScrollBox.OuterSingleton;
	}
	template<> MMA3_API UClass* StaticClass<UCustomScrollBox>()
	{
		return UCustomScrollBox::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UCustomScrollBox);
	struct Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_Components_CustomScrollBox_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_Components_CustomScrollBox_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UCustomScrollBox, UCustomScrollBox::StaticClass, TEXT("UCustomScrollBox"), &Z_Registration_Info_UClass_UCustomScrollBox, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCustomScrollBox), 3235133295U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_Components_CustomScrollBox_h_2740304121(TEXT("/Script/MMA3"),
		Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_Components_CustomScrollBox_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_Components_CustomScrollBox_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
