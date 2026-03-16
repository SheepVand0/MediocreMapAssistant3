#include "CController.h"

FString ACController::TimeToString(float sec) {

	int l_Hours = sec / 3600;
	int l_Minutes = (sec / 60) - (l_Hours * 60);
	int l_Seconds = (sec) - (l_Hours * 3600) - (l_Minutes * 60);
	int l_Milliseconds = ((float)sec - (int)sec) * 1000;

	FString l_Result;
	if (l_Hours > 0) {
		l_Result = ACController::MinZeros(FString::SanitizeFloat(l_Hours, 0), 2) + FString(":")
			+ ACController::MinZeros(FString::SanitizeFloat(l_Minutes, 0), 2) + FString(":")
			+ ACController::MinZeros(FString::SanitizeFloat(l_Seconds, 0), 2) + FString(":")
			+ ACController::MinZeros(FString::SanitizeFloat(l_Milliseconds, 0), 3);
	}
	else {
		l_Result = ACController::MinZeros(FString::SanitizeFloat(l_Minutes, 0), 2) + FString(":")
			+ ACController::MinZeros(FString::SanitizeFloat(l_Seconds, 0), 2) + FString(":")
			+ ACController::MinZeros(FString::SanitizeFloat(l_Milliseconds, 0), 3);
	}

	return l_Result;
}

FString ACController::MinZeros(FString value, int count) {

	int l_Len = value.Len();
	if (l_Len >= count) return value;

	for (int l_i = 0; l_i < count - l_Len; l_i++) {
		value = FString("0") + value;
	}

	return value;
}

ACController* ACController::GetControllerRef()
{
	return GetMappingController();
}
