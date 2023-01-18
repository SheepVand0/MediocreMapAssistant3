// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MMA3/MMA3GameModeBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMMA3GameModeBase() {}
// Cross Module References
	MMA3_API UClass* Z_Construct_UClass_AMMA3GameModeBase_NoRegister();
	MMA3_API UClass* Z_Construct_UClass_AMMA3GameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_MMA3();
// End Cross Module References
	void AMMA3GameModeBase::StaticRegisterNativesAMMA3GameModeBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AMMA3GameModeBase);
	UClass* Z_Construct_UClass_AMMA3GameModeBase_NoRegister()
	{
		return AMMA3GameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_AMMA3GameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AMMA3GameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_MMA3,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMMA3GameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "MMA3GameModeBase.h" },
		{ "ModuleRelativePath", "MMA3GameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AMMA3GameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AMMA3GameModeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AMMA3GameModeBase_Statics::ClassParams = {
		&AMMA3GameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_AMMA3GameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AMMA3GameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AMMA3GameModeBase()
	{
		if (!Z_Registration_Info_UClass_AMMA3GameModeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AMMA3GameModeBase.OuterSingleton, Z_Construct_UClass_AMMA3GameModeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AMMA3GameModeBase.OuterSingleton;
	}
	template<> MMA3_API UClass* StaticClass<AMMA3GameModeBase>()
	{
		return AMMA3GameModeBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AMMA3GameModeBase);
	struct Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_MMA3GameModeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_MMA3GameModeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AMMA3GameModeBase, AMMA3GameModeBase::StaticClass, TEXT("AMMA3GameModeBase"), &Z_Registration_Info_UClass_AMMA3GameModeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AMMA3GameModeBase), 2974762172U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_MMA3GameModeBase_h_538336833(TEXT("/Script/MMA3"),
		Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_MMA3GameModeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_MMA3_Source_MMA3_MMA3GameModeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
