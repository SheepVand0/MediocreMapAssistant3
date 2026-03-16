// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "MMA3/FastLog.h"
#include "RuntimeAudioImporter/Public/Sound/ImportedSoundWave.h"
#include "MMA3/MMAConfig.h"
//#include "RenderWaveForm.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNeedSpectrogramUpdate, TArray<float>&, frequencies);

/**
 * 
 */
class MMA3_API URenderWaveform : public UBlueprintFunctionLibrary
{
public:
	// Code stolen from MMA2
	static void CalculateFrequencySpectrum(UImportedSoundWave* InSoundWaveRef, const float InStartTime, const float InDuration, TArray<float, FDefaultAllocator64>& OutFrequencies);
};

class MMA3_API URenderSoundVisualization : public UBlueprintFunctionLibrary 
{

	//GENERATED_BODY()

public:

	//UPROPERTY()
	//static FOnNeedSpectrogramUpdate OnNeedSpectrogramUpdate;

	static void GenerateVisMesh(UProceduralMeshComponent* mesh, int verticesX, int verticesY, float sizeXMultiplier, float sizeYMultiplier, TArray<FVector>& outVertices, TArray<FLinearColor>& outVertexColors);

	static int GetIndexFromCoordinate(int x, int y, int sizeY);

	static void RenderSoundVis(UProceduralMeshComponent* mesh, UImportedSoundWave* sound, TArray<FVector> meshVertices, TArray<FLinearColor> meshVertexColors, float time, float duration, float itDuration, int subDivisionsX, int subDivisionsY);
};
