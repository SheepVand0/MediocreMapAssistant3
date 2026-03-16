// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VaRest\Public\VaRestJsonObject.h"
#include "VaRest\Public\VaRestJsonValue.h"
#include <string>

#define MARK_JSON_FLOAT(var) FloatObjects.Add(#var, &var);
#define MARK_JSON_STRING(var) StringObjects.Add(#var, &var);
#define MARK_JSON_OBJECT(var, objType) var = new objType(); \
Objects.Add(#var, var);
#define MARK_JSON_BOOL(var) BoolObjects.Add(#var, &var);
#define MARK_JSON_ARRAY(array) MarkArray(&array, #array);
//#define MARK_JSON_INT(var) IntObjects.Add(#var, &var);
#define MARK_JSON_INT64(var) Int64Objects.Add(#var, (int64*)&(var));

#define MARK_JSON_FLOAT_ALIASES(var, aliases) FloatObjects.Add(aliases, &var);
#define MARK_JSON_STRING_ALIASES(var, aliases) StringObjects.Add(aliases, &var);
#define MARK_JSON_OBJECT_ALIASES(var, aliases, objType) var = new objType(); Objects.Add(aliases, var);
#define MARK_JSON_BOOL_ALIASES(var, aliases) BoolObjects.Add(aliases, &var);
//#define MARK_JSON_INT_ALIASES(var, aliases) IntObjects.Add(aliases, &var);
#define MARK_JSON_INT64_ALIASES(var, aliases) Int64Objects.Add(aliases, (int64*)&(var));
#define MARK_JSON_ARRAY_ALIASES(array, aliases) MarkArray(&array, aliases);

#define MARK_SERIALIZE_IGNORED(var) SerializeIgnored.Add(#var);
#define MARK_SERIALIZE_IGNORED_ALIAS(alias) SerializeIgnored.Add(alias);

struct FJsonDataObject;
struct FStringArray;
	
template<typename t_dataType>
class FHolder {

public:

	FHolder() {}

	virtual t_dataType* getNew() {
		return new t_dataType();
	}

};

template<typename t_originalData, typename t_newData = t_originalData>
class FTemplatedHolder : public FHolder<t_originalData> {

public:

	virtual t_originalData* getNew() {
		return new t_newData();
	}

};

struct FStringArray {

public:

	FStringArray(TArray<FString> objs);

	FStringArray() : FStringArray(TArray<FString>{}) {

	}

	FStringArray(FString one) : FStringArray(TArray<FString>{ one }) {

	}

	TArray<FString> Objs;

	TArray<FString>& operator ()() {
		return Objs;
	}

	bool operator == (FStringArray x) {
		if (x.Objs.IsValidIndex(0))
			return Objs.Contains(x.Objs[0]);
		else
			return false;
	}

	FHolder<FJsonDataObject>* Template;
};

template<class t_valueType>
class FJsonObjectMap {

public:
	TArray<FStringArray> Keys;

	TArray<t_valueType> Values;

	FJsonObjectMap() {
		Keys = {};
		Values = {};
	}

	void Add(FString key, t_valueType value) {
		for (int x = 0; x < Values.Num(); x++) {
			if (Values[x] == value) {
				Keys[x].Objs.Add(key);
				return;
			}
		}

		Keys.Add(FStringArray(key));
		Values.Add(value);
	}

	template<typename t_dataType = FJsonDataObject>
	void Add(FString key, t_valueType value, FTemplatedHolder<FJsonDataObject, t_dataType>* templateHolder) {
		for (int x = 0; x < Values.Num(); x++) {
			if (Values[x] == value) {
				Keys[x].Objs.Add(key);
				Keys[x].Template = templateHolder;
				return;
			}
		}

		FStringArray l_Array = FStringArray(key);
		l_Array.Template = templateHolder;

		Keys.Add(l_Array);
		Values.Add(value);
	}

	void Add(FStringArray key, t_valueType value) {
		Keys.Add(key);
		Values.Add(value);
	}

	bool Contains(FStringArray key) {
		for (int x = 0; x < Keys.Num(); x++) {
			if (Keys[x] == key) {
				return true;
			}
		}

		return false;
	}

	bool Contains(FStringArray key, int& index) {
		int discarded = Keys.Num();

		for (int x = 0; x < Keys.Num(); x++) {
			if (Keys[x] == key) {
				index = x;
				return true;
			}
		}

		return false;
	}

	t_valueType& operator [](FStringArray y) {

		for (int x = 0; x < Keys.Num(); x++) {
			if (Keys[x] == y) {
				return Values[x];
			}
		}
		
		throw;
	}

};

#define PUSH_IN_ARRAY(array, name, as) if (array.Contains(name)) {\
	*array[FStringArray(name)] = as;\
}\

#define PARSE_ARRAY_FOR_SERIALIZATION(array, type) for (int x = 0; x < array.Keys.Num();x++) {\
	if (array.Values[x]) {\
		l_Object->Set##type##Field(array.Keys[x].Objs[0], *array.Values[x]);\
	}\
}\

struct FJsonDataObject {

public:

	FJsonDataObject();
	virtual ~FJsonDataObject();

	//FJsonDataObject(FJsonDataObject& x);

	FJsonObjectMap<float*> FloatObjects;

	FJsonObjectMap<int64*> Int64Objects;

	FJsonObjectMap<FString*> StringObjects;

	FJsonObjectMap<FJsonDataObject*> Objects;

	FJsonObjectMap<bool*> BoolObjects;	

	FJsonObjectMap<TArray<FJsonDataObject*>*> ObjectsArray;

	TArray<FString> SerializeIgnored;

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// Events handling

	virtual void PreParse() {};

	virtual void Init() {};

	virtual void PostParse() {};

//////////////////////////////////////////////////////////////////////////////////////////////////////

	void Parse(UVaRestJsonObject* x);

	FString Serialize();

	UVaRestJsonObject* SerializeToObject();

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// Tools

	template<class t_dataType = FJsonDataObject>
	void MarkArray(TArray<t_dataType*>* target, const char* targetName) {
		/*FStringArray l_Array = FStringArray(targetName);
		l_Array.Template = new FTemplatedHolder<FJsonDataObject, t_dataType>();*/

		ObjectsArray.Add<t_dataType>(FString(targetName), (TArray<FJsonDataObject*>*)target, new FTemplatedHolder<FJsonDataObject, t_dataType>());
	}
};