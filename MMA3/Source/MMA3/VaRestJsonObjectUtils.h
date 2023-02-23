// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C:\UE\UE5_1_1\UE_5.1\Engine\Plugins\Marketplace\VaRestPlugin\Source\VaRest\Public\VaRestJsonObject.h"
#include "C:\UE\UE5_1_1\UE_5.1\Engine\Plugins\Marketplace\VaRestPlugin\Source\VaRest\Public\VaRestJsonValue.h"

/**
 *
 */
class MMA3_API VaRestJsonObjectUtils
{
public:
	VaRestJsonObjectUtils();
	~VaRestJsonObjectUtils();

	static FString GetStringField(UVaRestJsonObject* p_Object, FString p_Name) {
		if (p_Object->HasField(p_Name)) {
			return p_Object->GetStringField(p_Name);
		}
		else {
			return FString("");
		}
	}

	static float GetNumberField(UVaRestJsonObject* p_Object, FString p_Name) {
		if (p_Object->HasField(p_Name)) {
			return p_Object->GetNumberField(p_Name);
		}
		else {
			return 0.0f;
		}
	}

	static int GetIntegerField(UVaRestJsonObject* p_Object, FString p_Name) {
		if (p_Object->HasField(p_Name)) {
			return p_Object->GetIntegerField(p_Name);
		}
		else {
			return 0;
		}
	}

	static UVaRestJsonValue* GetValueField(UVaRestJsonObject* p_Object, FString p_Name) {
		if (p_Object->HasField(p_Name)) {
			return p_Object->GetField(p_Name);
		}
		else {
			return nullptr;
		}
	}

	static TArray<UVaRestJsonValue*> GetValueArrayField(UVaRestJsonObject* p_Object, FString p_Name) {
		if (p_Object->HasField(p_Name)) {
			return p_Object->GetArrayField(p_Name);
		}
		else {
			return TArray<UVaRestJsonValue*>{};
		}
	}

	static TArray<FString> GetStringArrayField(UVaRestJsonObject* p_Object, FString p_Name) {
		if (p_Object->HasField(p_Name)) {
			return p_Object->GetStringArrayField(p_Name);
		}
		else {
			return TArray<FString>{};
		}
	}

	static TArray<float> GetNumberArrayField(UVaRestJsonObject* p_Object, FString p_Name) {
		if (p_Object->HasField(p_Name)) {
			return p_Object->GetNumberArrayField(p_Name);
		}
		else {
			return TArray<float>{};
		}
	}

	static TArray<int> GetIntegerArrayField(UVaRestJsonObject* p_Object, FString p_Name) {
		if (p_Object->HasField(p_Name)) {
			return p_Object->GetIntegerArrayField(p_Name);
		}
		else {
			return TArray<int>{};
		}
	}
};
