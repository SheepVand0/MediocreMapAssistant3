// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MMA3/Widgets/Components/CustomButton.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCustomButton() {}
// Cross Module References
	MMA3_API UClass* Z_Construct_UClass_UCustomButton_NoRegister();
	MMA3_API UClass* Z_Construct_UClass_UCustomButton();
	UMG_API UClass* Z_Construct_UClass_UButton();
	UPackage* Z_Construct_UPackage__Script_MMA3();
// End Cross Module References
	void UCustomButton::StaticRegisterNativesUCustomButton()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCustomButton);
	UClass* Z_Construct_UClass_UCustomButton_NoRegister()
	{
		return UCustomButton::StaticClass();
	}
	struct Z_Construct_UClass_UCustomButton_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UCustomButton_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UButton,
		(UObject* (*)())Z_Construct_UPackage__Script_MMA3,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCustomButton_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n *\n */" },
		{ "IncludePath", "Widgets/Components/CustomButton.h" },
		{ "ModuleRelativePath", "Widgets/Components/CustomButton.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UCustomButton_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCustomButton>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UCustomButton_Statics::ClassParams = {
		&UCustomButton::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UCustomButton_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UCustomButton_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UCustomButton()
	{
		if (!Z_Registration_Info_UClass_UCustomButton.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCustomButton.OuterSingleton, Z_Construct_UClass_UCustomButton_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UCustomButton.OuterSingleton;
	}
	template<> MMA3_API UClass* StaticClass<UCustomButton>()
	{
		return UCustomButton::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UCustomButton);
	struct Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_Components_CustomButton_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_Components_CustomButton_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UCustomButton, UCustomButton::StaticClass, TEXT("UCustomButton"), &Z_Registration_Info_UClass_UCustomButton, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCustomButton), 1941018893U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_Components_CustomButton_h_2438362411(TEXT("/Script/MMA3"),
		Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_Components_CustomButton_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_Widgets_Components_CustomButton_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
