// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MMA3/Widgets/Components/CustomSpinBox.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCustomSpinBox() {}
// Cross Module References
	MMA3_API UClass* Z_Construct_UClass_UCustomSpinBox_NoRegister();
	MMA3_API UClass* Z_Construct_UClass_UCustomSpinBox();
	UMG_API UClass* Z_Construct_UClass_USpinBox();
	UPackage* Z_Construct_UPackage__Script_MMA3();
// End Cross Module References
	void UCustomSpinBox::StaticRegisterNativesUCustomSpinBox()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCustomSpinBox);
	UClass* Z_Construct_UClass_UCustomSpinBox_NoRegister()
	{
		return UCustomSpinBox::StaticClass();
	}
	struct Z_Construct_UClass_UCustomSpinBox_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UCustomSpinBox_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_USpinBox,
		(UObject* (*)())Z_Construct_UPackage__Script_MMA3,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCustomSpinBox_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "Widgets/Components/CustomSpinBox.h" },
		{ "ModuleRelativePath", "Widgets/Components/CustomSpinBox.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UCustomSpinBox_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCustomSpinBox>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UCustomSpinBox_Statics::ClassParams = {
		&UCustomSpinBox::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UCustomSpinBox_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UCustomSpinBox_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UCustomSpinBox()
	{
		if (!Z_Registration_Info_UClass_UCustomSpinBox.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCustomSpinBox.OuterSingleton, Z_Construct_UClass_UCustomSpinBox_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UCustomSpinBox.OuterSingleton;
	}
	template<> MMA3_API UClass* StaticClass<UCustomSpinBox>()
	{
		return UCustomSpinBox::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UCustomSpinBox);
	struct Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_Components_CustomSpinBox_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_Components_CustomSpinBox_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UCustomSpinBox, UCustomSpinBox::StaticClass, TEXT("UCustomSpinBox"), &Z_Registration_Info_UClass_UCustomSpinBox, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCustomSpinBox), 103216940U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_Components_CustomSpinBox_h_617159135(TEXT("/Script/MMA3"),
		Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_Components_CustomSpinBox_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_Components_CustomSpinBox_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
