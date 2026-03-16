// Fill out your copyright notice in the Description page of Project Settings.


#include "RenderWaveForm.h"
#include "ThirdParty/Kiss_FFT/kiss_fft129/kiss_fft.h"
#include "ThirdParty/Kiss_FFT/kiss_fft129/tools/kiss_fftnd.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundWave.h"
#include "AudioDevice.h"
#include "Runtime/Engine/Public/VorbisAudioInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h"
#include "MMA3/Core/Controller/CController.h"

// I stole this function from MMA2 and modified it a little bit
void URenderWaveform::CalculateFrequencySpectrum(UImportedSoundWave* InSoundWaveRef, const float InStartTime, const float InDuration, TArray<float, FDefaultAllocator64>& OutFrequencies)
{
	if (!InSoundWaveRef) return;

	//UE_LOG(LogTemp, Display, TEXT("%p, %d"), InSoundWaveRef->RawPCMData, InSoundWaveRef->RawPCMDataSize);


	// Clear the Array before continuing
	OutFrequencies.Empty();

	uint16 NumChannels = InSoundWaveRef->NumChannels;
	int32 SampleRate = InSoundWaveRef->GetSampleRate();

	// Make sure the Number of Channels is correct
	if (NumChannels > 0 && NumChannels <= 2)
	{
		TArrayView64<float> l_PCMData = InSoundWaveRef->GetPCMBuffer().PCMData.GetView();

		// Check if we actually have a Buffer to work with
		if (l_PCMData.Num() > 0)
		{
			// The first sample is just the StartTime * SampleRate
			int32 FirstSample = SampleRate * (InStartTime);

			// The last sample is the SampleRate times (StartTime plus the Duration)
			int32 LastSample = SampleRate * ((InStartTime)+InDuration);

			// Get Maximum amount of samples in this Sound
			const int32 SampleCount = InSoundWaveRef->GetPCMBuffer().PCMData.GetView().Num() * sizeof(float) / (2 * NumChannels);

			//UE_LOG(LogTemp, Display, TEXT("Sample Count : %d"), SampleCount);

			// An early check if we can create a Sample window
			FirstSample = FMath::Min(SampleCount, FirstSample);
			LastSample = FMath::Min(SampleCount, LastSample);

			// Actual amount of samples we gonna read
			int32 SamplesToRead = LastSample - FirstSample;

			if (SamplesToRead <= 0) {

				//PrintError(TEXT("Number of SamplesToRead is < 0!"));
				return;
			}

			// Shift the window enough so that we get a PowerOfTwo. FFT works better with that
			int32 PoT = 1;

			/*while (SamplesToRead > PoT) {
				PoT *= 2;
			}*/

			while (PoT << 1 <= SamplesToRead) PoT <<= 1;

			// Now we have a good PowerOfTwo to work with
			SamplesToRead = PoT * 2;

			// Create two 2-dim Arrays for complex numbers | Buffer and Output
			kiss_fft_cpx* Buffer[2] = { 0 };
			kiss_fft_cpx* Output[2] = { 0 };

			// Create 1-dim Array with one slot for SamplesToRead
			int32 Dims[1] = { SamplesToRead };

			// alloc once and forget, should probably move to a init/deinit func
			static kiss_fftnd_cfg STF = kiss_fftnd_alloc(Dims, 1, 0, nullptr, nullptr);

			//int16* SamplePtr = reinterpret_cast<int16*>(l_PCMData);
			float* SamplePtr = l_PCMData.GetData();

			if (!SamplePtr) return;

			// Allocate space in the Buffer and Output Arrays for all the data that FFT returns
			for (int32 ChannelIndex = 0; ChannelIndex < NumChannels; ChannelIndex++)
			{
				Buffer[ChannelIndex] = (kiss_fft_cpx*)calloc(SamplesToRead, sizeof(kiss_fft_cpx));
				Output[ChannelIndex] = (kiss_fft_cpx*)calloc(SamplesToRead, sizeof(kiss_fft_cpx));
			}

			// Shift our SamplePointer to the Current "FirstSample"
			SamplePtr += FirstSample * NumChannels;

			float precomputeMultiplier = 2.f * PI / (SamplesToRead - 1);

			for (int32 SampleIndex = 0; SampleIndex < SamplesToRead; SampleIndex++)
			{
				float rMult = 0.f;

				if (SamplePtr != nullptr && (SampleIndex + FirstSample < SampleCount))
				{
					rMult = 0.5f * (1.f - FMath::Cos(precomputeMultiplier * SampleIndex));
				}
				for (int32 ChannelIndex = 0; ChannelIndex < NumChannels; ChannelIndex++)
				{
					// Make sure the Point is Valid and we don't go out of bounds
					if (SamplePtr != nullptr && (SampleIndex + FirstSample < SampleCount))
					{
						// Use Window function to get a better result for the Data (Hann Window)
						Buffer[ChannelIndex][SampleIndex].r = rMult * (*SamplePtr);
					}
					else
					{
						Buffer[ChannelIndex][SampleIndex].r = 0.f;
					}
					Buffer[ChannelIndex][SampleIndex].i = 0.f;

					// Take the next Sample
					SamplePtr++;
				}
			}

			// Now that the Buffer is filled, use the FFT
			for (int32 ChannelIndex = 0; ChannelIndex < NumChannels; ChannelIndex++)
			{
				if (Buffer[ChannelIndex])
				{
					kiss_fftnd(STF, Buffer[ChannelIndex], Output[ChannelIndex]);
				}
			}

			OutFrequencies.AddZeroed(SamplesToRead);

			for (int32 SampleIndex = 0; SampleIndex < SamplesToRead; ++SampleIndex)
			{
				float ChannelSum = 0.0f;

				for (int32 ChannelIndex = 0; ChannelIndex < NumChannels; ++ChannelIndex)
				{
					if (Output[ChannelIndex])
					{
						// With this we get the actual Frequency value for the frequencies from 0hz to ~22000hz
						float l_Freq1 = FMath::Sqrt(FMath::Square(Output[ChannelIndex][SampleIndex].r) + FMath::Square(Output[ChannelIndex][SampleIndex].i));
						float l_Freq = (1000 / log(2)) * log(1 + (l_Freq1 / 1000));

						if (l_Freq <= GetMappingController()->HighPassFilter) {
							l_Freq *= (l_Freq / GetMappingController()->HighPassFilter);
						} else
						if (l_Freq >= GetMappingController()->LowPassFilter) {
							l_Freq *= (l_Freq / GetMappingController()->LowPassFilter);
						}
						else {
							float l_Window = GetMappingController()->LowPassFilter - GetMappingController()->HighPassFilter;

							if (l_Freq <= GetMappingController()->HighPassFilter + (l_Window / 2.f)) {
								l_Freq *= (l_Freq - GetMappingController()->HighPassFilter) / (l_Window / 2.f);
							}
							else {
								l_Freq *= (l_Freq - GetMappingController()->HighPassFilter + (l_Window / 2.f)) / (l_Window / 2.f);
							}
						}


						ChannelSum += l_Freq;
					}
				}

				

				if (GetConfig()->LogarithmicFrequency && false)
				{
					OutFrequencies[SampleIndex] = FMath::LogX(10, ChannelSum / NumChannels) * 10;
				}
				else
				{
					OutFrequencies[SampleIndex] = ChannelSum / NumChannels;

				}
			}

			for (int32 ChannelIndex = 0; ChannelIndex < NumChannels; ++ChannelIndex)
			{
				if (Buffer[ChannelIndex] != nullptr)
					KISS_FFT_FREE(Buffer[ChannelIndex]);
				if (Output[ChannelIndex] != nullptr)
					KISS_FFT_FREE(Output[ChannelIndex]);
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Cannot get PCMData!"));
		}
	}
	else {
	}

}

void URenderSoundVisualization::GenerateVisMesh(UProceduralMeshComponent* mesh, int verticesX, int verticesY, float sizeXMultiplier, float sizeYMultiplier, TArray<FVector>& outVertices, TArray<FLinearColor>& outVertexColors)
{
	TArray<FVector> l_Vertices;
	TArray<int> l_Triangles;
	TArray<FLinearColor> l_VertexColor;

	//UE_LOG(LogTemp, Warning, TEXT("Generating Vis Mesh, VX: %d, VY: %d"), verticesX, verticesY);

	//l_Vertices.Add(FVector(0, 0, 0));
	//l_Vertices.Add(FVector(0, sizeYMultiplier, 0));

	for (int l_x = 0; l_x < verticesX; l_x++) {
		for (int l_y = 0; l_y < verticesY; l_y++) {
			l_Vertices.Add(FVector(l_x * sizeXMultiplier, l_y * sizeYMultiplier, 0));
			l_VertexColor.Add(FLinearColor::Black);
			//l_VertexColor.Add(FLinearColor(FMath::RandRange(0.f, 1.f), FMath::RandRange(0.f, 1.f), FMath::RandRange(0.f, 1.f)));
		}
	}

	for (int l_x = 0; l_x < verticesX - 1; l_x++) {
		for (int l_y = 0; l_y < verticesY - 1; l_y++) {
			int l_BottomLeftIndex = l_x * verticesY + l_y;
			int l_BottomRightIndex = l_x * verticesY + l_y + 1;
			int l_TopLeftIndex = (l_x + 1) * verticesY + l_y;
			int l_TopRightIndex = (l_x + 1) * verticesY + l_y + 1;
			//UE_LOG(LogTemp, Warning, TEXT("l_x : %d, l_y: %d"), l_x, l_y);

			FVector l_BottomLeftVertice = l_Vertices[l_BottomLeftIndex];
			FVector l_BottomRightVertice = l_Vertices[l_BottomRightIndex];
			FVector l_TopLeftVertice = l_Vertices[l_TopLeftIndex];
			FVector l_TopRightVertice = l_Vertices[l_TopRightIndex];

			// First square triangle
			l_Triangles.Add(l_BottomLeftIndex);
			l_Triangles.Add(l_BottomRightIndex);
			l_Triangles.Add(l_TopLeftIndex);

			// Second square triangle
			l_Triangles.Add(l_TopRightIndex);
			l_Triangles.Add(l_TopLeftIndex);
			l_Triangles.Add(l_BottomRightIndex);
		}
	}

	mesh->CreateMeshSection_LinearColor(0, l_Vertices, l_Triangles, TArray<FVector>(), TArray<FVector2D>(), l_VertexColor, TArray<FProcMeshTangent>(), false);

	outVertices = l_Vertices;
	outVertexColors = l_VertexColor;
}

int URenderSoundVisualization::GetIndexFromCoordinate(int x, int y, int sizeY)
{
	return (x * sizeY) + y;
}

void URenderSoundVisualization::RenderSoundVis(UProceduralMeshComponent* mesh, UImportedSoundWave* sound, TArray<FVector> meshVertices, TArray<FLinearColor> meshVertexColors, float time, float duration, float itDuration, int subDivisionsX, int subDivisionsY)
{
	if (!mesh || !sound) {
		return;
	}

	float l_DivDuration = duration / (float)subDivisionsX;
	float l_ItDuration = itDuration;
	//float l_ItDuration = 1 / 64.f;
	TArray<FLinearColor> l_VertexColor;
	TArray<FVector> l_Vertices = TArray<FVector>(meshVertices);
	l_VertexColor.AddZeroed(meshVertexColors.Num());

	//UE_LOG(LogTemp, Display, TEXT("Updatin SoundVis, time: %f, duration: %f, itDuration: %f, dX: %d, dY: %d"), time, duration, itDuration, subDivisionsX, subDivisionsY);

	for (int l_x = 0; l_x < subDivisionsX; l_x++) {

		float l_Math = time + ((float)l_x * l_DivDuration);

		TArray<float, FDefaultAllocator64> l_Frequencies;

		if (l_Math >= sound->Duration) break;

		//UE_LOG(LogTemp, Display, TEXT("l_x: %d"), l_x);
		URenderWaveform::CalculateFrequencySpectrum(sound, l_Math, l_ItDuration, l_Frequencies);

		float l_Last = 0.f;

		for (int l_y = 0; l_y < subDivisionsY; l_y++) {
			float l_Height;
			if (l_Frequencies.Num() > (l_y * 8.f))
				l_Height = l_Frequencies[l_y * 8.f];
			else
				l_Height = 0.f;

			/*if (l_Height == 0)
				l_Height = l_Last;
			else
				l_Last = l_Height;*/

				/*GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Blue, FString::SanitizeFloat(l_Height));
				if (FMath::Abs(l_Height) <= GetMappingController()->HighPassFilter) l_Height = 0.f;
				if (FMath::Abs(l_Height) >= GetMappingController()->LowPassFilter) l_Height = 0.f;*/


			int l_I = GetIndexFromCoordinate(l_x, l_y, subDivisionsY);
			if (l_VertexColor.IsValidIndex(l_I))
				l_VertexColor[l_I] = FLinearColor(FMath::Abs(l_Height) / (GetConfig()->SoundVisAttenuation == 0 ? 0.00001f : GetConfig()->SoundVisAttenuation), 0, 0);
			if (l_Vertices.IsValidIndex(l_I))
				l_Vertices[l_I].Z = FMath::Clamp(l_Height, 0, 0.5f);
		}
	}

	mesh->UpdateMeshSection_LinearColor(0, l_Vertices, TArray<FVector>(), TArray<FVector2D>(), l_VertexColor, TArray<FProcMeshTangent>());
}
