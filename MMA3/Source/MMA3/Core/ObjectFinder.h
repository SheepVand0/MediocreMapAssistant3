// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
template<typename T = UObject>
class MMA3_API ObjectFinder
{
private:

	T* Object;

public:
	ObjectFinder() {}
	~ObjectFinder() {}

	ObjectFinder(const TCHAR* p_Text) {
		ConstructorHelpers::FObjectFinder<T> l_Object(p_Text);
		Object = l_Object.Object;
	};

	T* GetObject() {
		return Object;
	}
};
