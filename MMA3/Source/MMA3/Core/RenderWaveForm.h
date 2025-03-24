// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "MMA3/FastLog.h"
#include "RuntimeAudioImporter/Public/Sound/ImportedSoundWave.h"
#include "MMA3/Core/EnvironmentSpectrograms.h"

/**
 * 
 */
class MMA3_API URenderWaveform : public UBlueprintFunctionLibrary
{
public:
	// Code stolen from MMA2
	static void CalculateFrequencySpectrum(UImportedSoundWave* InSoundWaveRef, TArray<uint8> pcmData, int32 samplesRate, const float InStartTime, const float InDuration, TArray<float>& OutFrequencies);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Render Spectrogram"), Category = "Render Spectrogram")
	static void RenderWaveform(UImportedSoundWave* InSoundWaveRef, TArray<uint8> pcmData, int32 samplesRate, UProceduralMeshComponent* Mesh, float InSongPosition, int SizeX);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Spectrogram Mesh"), Category = "Render Spectrogram")
	static void GenerateSpectrogramMesh(UProceduralMeshComponent* Mesh, int SizeX, int SizeY);

	static int To1D(int x, int y, int sizeX);
};
