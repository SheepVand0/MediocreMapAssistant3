// Fill out your copyright notice in the Description page of Project Settings.


#include "CVaRestHelper.h"
#include "VaRest\Public\VaRestSubsystem.h"
#include <exception>

FJsonDataObject::FJsonDataObject() : FloatObjects(), StringObjects(), BoolObjects(), Objects(), ObjectsArray(), Int64Objects()
{

}

FJsonDataObject::~FJsonDataObject()
{

}

//FJsonDataObject::FJsonDataObject(FJsonDataObject& x)
//{
//	BoolObjects = x.BoolObjects;
//	FloatObjects = x.FloatObjects;
//	StringObjects = x.StringObjects;
//	Objects = x.Objects;
//	ObjectsArray = x.ObjectsArray;
//}

void FJsonDataObject::Parse(UVaRestJsonObject* x)
{
	auto l_Names = x->GetFieldNames();
	int out = 0;

	for (auto l_Name : l_Names) {
		UVaRestJsonValue* l_AsValue = x->GetField(l_Name);
		int index = 0;
		switch (l_AsValue->GetType()) {
		case EVaJson::Boolean:
			PUSH_IN_ARRAY(BoolObjects, l_Name, l_AsValue->AsBool());
			break;
		case EVaJson::Number:
			PUSH_IN_ARRAY(FloatObjects, l_Name, l_AsValue->AsNumber())
			else PUSH_IN_ARRAY(Int64Objects, l_Name, l_AsValue->AsInt64())
				break;
		case EVaJson::String:
			if (StringObjects.Contains(l_Name)) {
				*StringObjects[FStringArray(l_Name)] = l_AsValue->AsString();
				UE_LOG(LogTemp, Display, TEXT("Parsed: %s"), **StringObjects.Values[out]);
			}
			break;
		case EVaJson::Object:
			if (Objects.Contains(l_Name)) {
				if (Objects[l_Name]) {
					Objects[l_Name]->Parse(l_AsValue->AsObject());
				}
			}
			break;
		case EVaJson::Array:
			//UE_LOG(LogTemp, Display, TEXT("at least went here"));
			if (ObjectsArray.Contains(l_Name, index)) {
				TArray<UVaRestJsonValue*> l_Values = l_AsValue->AsArray();

				//ObjectsArray[l_Name]->AddDefaulted(l_Values.Num());

				//UE_LOG(LogTemp, Display, TEXT("Count: %d"), l_Values.Num());

				for (int y = 0; y < l_Values.Num(); y++) {
					ObjectsArray[l_Name]->Add(ObjectsArray.Keys[index].Template->getNew());

					//UE_LOG(LogTemp, Display, TEXT("Value names count: %d"), ObjectsArray.Keys[index].Template->getNew()->FloatObjects.Keys.Num());

					ObjectsArray[l_Name]->GetData()[y]->Parse(l_Values[y]->AsObject());
					//UE_LOG(LogTemp, Display, TEXT("time: %f"), *(ObjectsArray[l_Name]->GetData()[y]->FloatObjects[FStringArray("b")]));
				}
			}
			break;
		default: break;
		}

	}

	PostParse();
}

UVaRestJsonObject* FJsonDataObject::SerializeToObject()
{
	PreParse();

	UVaRestSubsystem* l_System = GEngine->GetEngineSubsystem<UVaRestSubsystem>();
	UVaRestJsonObject* l_Object = l_System->ConstructVaRestJsonObject();

	PARSE_ARRAY_FOR_SERIALIZATION(BoolObjects, Bool);
	PARSE_ARRAY_FOR_SERIALIZATION(FloatObjects, Number);
	PARSE_ARRAY_FOR_SERIALIZATION(Int64Objects, Number);
	
	for (int x = 0; x < StringObjects.Keys.Num(); x++) {
		if (StringObjects.Values[x]) {
			UE_LOG(LogTemp, Display, TEXT("uzerfpuirzefbf: %s %p"), *(StringObjects.Keys[x].Objs[0]), StringObjects.Values[x]);
			UE_LOG(LogTemp, Display, TEXT("uzerfpuirzefbf: %s %s"), *(StringObjects.Keys[x].Objs[0]), **(StringObjects.Values[x]));
			if (!StringObjects.Keys[x].Objs.IsEmpty()) {
				FString l_String = *(StringObjects.Values[x]);
				
				l_Object->SetStringField(StringObjects.Keys[x].Objs[0], l_String);
			}
		}
	};

	for (int x = 0; x < Objects.Keys.Num(); x++) {
		if (Objects.Values[x]) {
			l_Object->SetObjectField(Objects.Keys[x].Objs[0], Objects.Values[x]->SerializeToObject());
		}
	};

	for (int x = 0; x < ObjectsArray.Keys.Num(); x++) {
		TArray<UVaRestJsonObject*> l_List;

		for (int y = 0; y < ObjectsArray.Values[x]->Num(); y++) {
			if (ObjectsArray.Values[x]) {
				if (ObjectsArray.Values[x]->GetData()[y])
				l_List.Add(ObjectsArray.Values[x]->GetData()[y]->SerializeToObject());
			}
		}

		l_Object->SetObjectArrayField(ObjectsArray.Keys[x].Objs[0], l_List);
	}

	return l_Object;
}

FString FJsonDataObject::Serialize()
{
	return SerializeToObject()->EncodeJson();
}

FStringArray::FStringArray(TArray<FString> objs)
{
	Objs = objs;
	Template = nullptr;
}
