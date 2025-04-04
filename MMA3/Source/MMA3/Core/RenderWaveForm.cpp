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

// All code of this class is stolen from MMA2

void URenderWaveform::CalculateFrequencySpectrum(UImportedSoundWave* InSoundWaveRef, const float InStartTime, const float InDuration, TArray<float>& OutFrequencies)
{
	// Clear the Array before continuing
	OutFrequencies.Empty();

	uint16 NumChannels = InSoundWaveRef->NumChannels;
	int32 SampleRate = InSoundWaveRef->GetSampleRate();

	// Make sure the Number of Channels is correct
	if (NumChannels > 0 && NumChannels <= 2)
	{
		TArrayView<float, signed long long> l_PCMData = InSoundWaveRef->GetPCMBuffer().PCMData.GetView();
		
		// Check if we actually have a Buffer to work with
		if (l_PCMData.Num() > 0)
		{
			// The first sample is just the StartTime * SampleRate
			int32 FirstSample = SampleRate * (InStartTime);

			// The last sample is the SampleRate times (StartTime plus the Duration)
			int32 LastSample = SampleRate * ((InStartTime) + InDuration);

			// Get Maximum amount of samples in this Sound
			const int32 SampleCount = l_PCMData.Num() / (2 * NumChannels);
			//UE_LOG(LogTemp, Display, TEXT("Sample Count : %d"), SampleCount);

			// An early check if we can create a Sample window
			FirstSample = FMath::Min(SampleCount, FirstSample);
			LastSample = FMath::Min(SampleCount, LastSample);

			// Actual amount of samples we gonna read
			int32 SamplesToRead = LastSample - FirstSample;

			if (SamplesToRead < 0) {
				
				//PrintError(TEXT("Number of SamplesToRead is < 0!"));
				return;
			}

			// Shift the window enough so that we get a PowerOfTwo. FFT works better with that
			int32 PoT = 2;

			while (SamplesToRead > PoT) {
				PoT *= 2;
			}

			// Now we have a good PowerOfTwo to work with
			SamplesToRead = PoT;

			// Create two 2-dim Arrays for complex numbers | Buffer and Output
			kiss_fft_cpx* Buffer[2] = { 0 };
			kiss_fft_cpx* Output[2] = { 0 };

			// Create 1-dim Array with one slot for SamplesToRead
			int32 Dims[1] = { SamplesToRead };

			// alloc once and forget, should probably move to a init/deinit func
			static kiss_fftnd_cfg STF = kiss_fftnd_alloc(Dims, 1, 0, nullptr, nullptr);

			//int16* SamplePtr = reinterpret_cast<int16*>(l_PCMData);
			float* SamplePtr = l_PCMData.GetData();

			// Allocate space in the Buffer and Output Arrays for all the data that FFT returns
			for (int32 ChannelIndex = 0; ChannelIndex < NumChannels; ChannelIndex++)
			{
				Buffer[ChannelIndex] = (kiss_fft_cpx*)KISS_FFT_MALLOC(sizeof(kiss_fft_cpx) * SamplesToRead);
				Output[ChannelIndex] = (kiss_fft_cpx*)KISS_FFT_MALLOC(sizeof(kiss_fft_cpx) * SamplesToRead);
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
						ChannelSum += FMath::Sqrt(FMath::Square(Output[ChannelIndex][SampleIndex].r) + FMath::Square(Output[ChannelIndex][SampleIndex].i));
					}
				}

				if (false)
				{
					OutFrequencies[SampleIndex] = FMath::LogX(10, ChannelSum / NumChannels) * 10;
				}
				else
				{
					OutFrequencies[SampleIndex] = ChannelSum / NumChannels;
					
				}
			}

			// Make sure to free up the FFT stuff
			// KISS_FFT_FREE(STF)

			for (int32 ChannelIndex = 0; ChannelIndex < NumChannels; ++ChannelIndex)
			{
				if (Buffer[ChannelIndex])
					KISS_FFT_FREE(Buffer[ChannelIndex]);
				if (Output[ChannelIndex])
					KISS_FFT_FREE(Output[ChannelIndex]);
			}

		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Cannot get PCMData!"));
			//PrintError(TEXT("InSoundVisData.PCMData is a nullptr!"));
		}
	}
	else {
		//PrintError(TEXT("Number of Channels is < 0!"));
	}

}

TArray<float> URenderWaveform::CalculateFrequencySpectrum2(UImportedSoundWave* sound, TArrayView<float, signed long long> pcmData, int32 sampleRate, float startTime, float duration)
{
	int32 l_FrameRate = sampleRate / sound->NumChannels;
	int32 l_FirstFrame = FMath::Max((startTime) * l_FrameRate, 0);
	
	int32 l_TotalFrameCount = l_FrameRate * sound->GetDuration();

	int32 l_LastFrame = FMath::Min((startTime + duration) * l_FrameRate, l_TotalFrameCount);

	int32 l_ChannelsNum = sound->GetNumOfChannels();
	int32 l_LastIndex = l_FirstFrame + ((duration * l_FrameRate) * l_ChannelsNum);

	TArray<float> l_Result;
	int l_LastSign = 0;
	int32 l_LastSignChangeSample = 0;
	
	/*for (int l_i = l_FirstSample; l_i < l_LastSample; l_i += sound->NumChannels) {

		float l_Sum = 0;

		for (int l_c = 0; l_c < sound->NumChannels; l_c++) {
			l_Sum += pcmData[l_i + l_c];
		}

		if (l_LastSign == 0)
			l_LastSign = l_Sum;

		if (l_LastSign < 0 && l_Sum > 0) {
			l_LastSign = 1;
		}

		if (l_Sum < 0 && l_LastSign > 0) {
			l_LastSign = -1;
			
			if (l_i != l_LastSignChangeSample) {
				l_Frequencies.Add(((float)sampleRate / (l_i - l_LastSignChangeSample)));
			}
			l_LastSignChangeSample = l_i;
		}
	}*/

	for (int l_Frame = l_FirstFrame; l_Frame < l_LastFrame; l_Frame += l_ChannelsNum) {
		float l_Sum = 0;

		for (int l_Channel = 0; l_Channel < l_ChannelsNum; l_Channel++) {
			
			float l_Value = FMath::Abs(pcmData[l_Frame + l_Channel]);
			l_Sum += l_Value;
		}

		l_Result.Add(l_Sum);
	}

	l_Result.Sort();

	return l_Result;
}


void URenderWaveform::RenderWaveform(UImportedSoundWave* InSoundWaveRef, uint8* pcmData, int32 pcmDataCount, int32 samplesRate, UProceduralMeshComponent* Mesh, float InSongPosition, int SizeX) {
	if (!IsValid(InSoundWaveRef)) {
		return;
	}
	if (!IsValid(Mesh)) {
		return;
	}

	int nbVert = Mesh->GetProcMeshSection(0)->ProcVertexBuffer.Num();
	bool valid;

	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	Vertices.AddDefaulted(nbVert);
	Normals.Init(FVector(0.0f, 0.0f, 1.0f), nbVert);
	UV0.AddDefaulted(nbVert);
	VertexColors.AddDefaulted(nbVert);
	Tangents.Init(FProcMeshTangent(1.0f, 0.0f, 0.0f), nbVert);

	float duration = (1 / 64.f);
	for (size_t i = 0; i < 160; ++i) {
		float startTime = duration * i + InSongPosition;

		valid = true;
		if (startTime < 0.0f || startTime >= InSoundWaveRef->Duration || startTime + duration >= InSoundWaveRef->Duration) {
			valid = false;
		}

		TArray<float> results;

		if (valid) {
			CalculateFrequencySpectrum(InSoundWaveRef, startTime, duration, results);
			/*results = CalculateFrequencySpectrum2(
				InSoundWaveRef, 
				InSoundWaveRef->GetPCMBuffer().PCMData.GetView(), 
				InSoundWaveRef->GetSampleRate(), 
				startTime, 
				duration);*/
		}

		//for (int l_i = 0; l_i < results.Num(); l_i++)
		//	UE_LOG(LogTemp, Display, TEXT("Frequency spectrum [%d] : %f"), l_i, results[l_i]);

		TArray<float> l_SpectrogramsValue = TArray<float>();
		

		for (size_t j = 0; j < 64; ++j) {
			float height;

			if (valid && results.Num() > (j * 8.f)) height = results[j * 8.f] / 500/*/ 5000.f*/;
			else height = 0;

			if (Vertices.IsValidIndex(To1D(i, j, SizeX))) {
				Vertices[To1D(i, j, SizeX)] = FVector(i, j, height * 2.f);
				VertexColors[To1D(i, j, SizeX)] = FLinearColor(height, 0.0f, 0);
				l_SpectrogramsValue.Add(height);
			}
		}

		for (int l_i = 0; l_i < AEnvironmentSpectrograms::s_Instances.Num(); l_i++) {
			AEnvironmentSpectrograms* l_Item = AEnvironmentSpectrograms::s_Instances[l_i];
			l_Item->SetCurrentFrequency(l_SpectrogramsValue);
			//UE_LOG(LogTemp, Warning, TEXT("Number of Spectrograms found : %d"), AEnvironmentSpectrograms::s_Instances.Num())
		}
	}

	

	Mesh->UpdateMeshSection_LinearColor(0, Vertices, Normals, UV0, VertexColors, Tangents);

	return;
}

void URenderWaveform::GenerateSpectrogramMesh(UProceduralMeshComponent* Mesh, int SizeX, int SizeY)
{
	if (!IsValid(Mesh) || SizeX <= 0 || SizeY <= 0) {
		return;
	}

	TArray<FVector> Vertices;
	TArray<int> Faces;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	Vertices.AddDefaulted(SizeX * SizeY);
	Normals.AddDefaulted(SizeX * SizeY);
	UV0.AddDefaulted(SizeX * SizeY);
	VertexColors.AddDefaulted(SizeX * SizeY);
	Tangents.AddDefaulted(SizeX * SizeY);
	Faces.AddZeroed((SizeX - 1) * (SizeY - 1) * 6);

	for (int j = 0; j < SizeY; ++j)
	{
		for (int i = 0; i < SizeX; ++i)
		{
			Vertices[To1D(i, j, SizeX)] = FVector(i, j, 0.0f);
			Normals[To1D(i, j, SizeX)] = FVector(0.0f, 0.0f, 1.0f);
			UV0[To1D(i, j, SizeX)] = FVector2D(0.0f, 0.0f);
			VertexColors[To1D(i, j, SizeX)] = FLinearColor(0.0f, 0.0f, 0.0f);
			Tangents[To1D(i, j, SizeX)] = FProcMeshTangent(1.0f, 0.0f, 0.0f);
		}
	}

	for (int j = 0; j < SizeY - 1; ++j)
	{
		for (int i = 0; i < SizeX - 1; ++i)
		{
			Faces[To1D(i, j, SizeX - 1) * 6] = To1D(i, j, SizeX);
			Faces[To1D(i, j, SizeX - 1) * 6 + 1] = To1D(i, j + 1, SizeX);
			Faces[To1D(i, j, SizeX - 1) * 6 + 2] = To1D(i + 1, j, SizeX);

			Faces[To1D(i, j, SizeX - 1) * 6 + 3] = To1D(i + 1, j, SizeX);
			Faces[To1D(i, j, SizeX - 1) * 6 + 4] = To1D(i, j + 1, SizeX);
			Faces[To1D(i, j, SizeX - 1) * 6 + 5] = To1D(i + 1, j + 1, SizeX);

		}
	}

	Mesh->CreateMeshSection_LinearColor(0, Vertices, Faces, Normals, UV0, VertexColors, Tangents, false);
}

int URenderWaveform::To1D(int x, int y, int sizeX)
{
	return (sizeX * y) + x;
}

void URenderSoundVisualization::GenerateVisMesh(UProceduralMeshComponent* mesh, int verticesX, int verticesY, float sizeXMultiplier, float sizeYMultiplier, TArray<FVector>& outVertices, TArray<FLinearColor>& outVertexColors)
{
	TArray<FVector> l_Vertices;
	TArray<int> l_Triangles;
	TArray<FLinearColor> l_VertexColor;

	//l_Vertices.Add(FVector(0, 0, 0));
	//l_Vertices.Add(FVector(0, sizeYMultiplier, 0));

	for (int l_x = 0; l_x < verticesX; l_x++) {
		for (int l_y = 0; l_y < verticesY; l_y++) {
			l_Vertices.Add(FVector(l_x * sizeXMultiplier, l_y * sizeYMultiplier, 0));
			l_VertexColor.Add(FLinearColor::Black);
			//l_VertexColor.Add(FLinearColor(FMath::RandRange(0.f, 1.f), FMath::RandRange(0.f, 1.f), FMath::RandRange(0.f, 1.f)));
		}
	}

	for (int l_x = 0; l_x < verticesX - 1; l_x ++) {
		for (int l_y = 0; l_y < verticesY - 1; l_y++) {
			int l_BottomLeftIndex = l_y * verticesY + l_x;
			int l_BottomRightIndex = l_y * verticesY + l_x + 1;
			int l_TopLeftIndex = (l_y + 1) * verticesY + l_x;
			int l_TopRightIndex = (l_y + 1) * verticesY + l_x + 1;

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
			l_Triangles.Add(l_BottomRightIndex);
			l_Triangles.Add(l_TopLeftIndex);

		}
	}

	mesh->CreateMeshSection_LinearColor(0, l_Vertices, l_Triangles, TArray<FVector>(), TArray<FVector2D>(), l_VertexColor, TArray<FProcMeshTangent>(), false);

	outVertices = l_Vertices;
	outVertexColors = l_VertexColor;
}

float URenderSoundVisualization::GetMaxAmpForDuration(UImportedSoundWave* sound, float startTime, float duration)
{
	int32 l_SampleRate = sound->GetSampleRate();

	int32 l_StartSample = l_SampleRate * startTime * sound->NumChannels;
	int32 l_EndSample = l_StartSample + (l_SampleRate * duration * sound->NumChannels);

	float l_Result = 0;

	TArrayView<float, signed long long> l_PCMData = sound->GetPCMBuffer().PCMData.GetView();

	for (int l_i = l_StartSample; l_i < l_EndSample; l_i++) {
		float l_Sample = l_PCMData[l_i];

		l_Result = FMath::Max(l_Result, l_Sample);
	}

	return l_Result;
}

TArray<float> URenderSoundVisualization::GetFrequenciesForDuration(UImportedSoundWave* sound, float startTime, float duration)
{
	int32 l_SampleRate = sound->GetSampleRate();

	int32 l_StartSample = l_SampleRate * startTime;
	int32 l_EndSample = l_StartSample + (l_SampleRate * duration);

	TArray<float> l_Frequencies;

	TArrayView<float, signed long long> l_PCMData = sound->GetPCMBuffer().PCMData.GetView();

	int l_LastSign = 0;
	int32 l_LastSignChangeSample = 0;

	for (int l_i = l_StartSample; l_i < l_EndSample; l_i += sound->NumChannels) {

		float l_Sum = 0;

		for (int l_c = 0; l_c < sound->NumChannels; l_c++) {
			l_Sum += l_PCMData[l_i + l_c];
		}

		if (l_LastSign == 0)
			l_LastSign = l_Sum;

		if (l_LastSign < 0 && l_Sum > 0) {
			l_LastSign = 1;
		}

		if (l_Sum < 0 && l_LastSign > 0) {
			l_LastSign = -1;

			if (l_i != l_LastSignChangeSample) {
				l_Frequencies.Add(((float)l_SampleRate / (l_i - l_LastSignChangeSample)));
			}
			l_LastSignChangeSample = l_i;
		}
	}

	return l_Frequencies;
}

int URenderSoundVisualization::GetIndexFromCoordinate(int x, int y, int sizeY)
{
	return y * sizeY + x;
}

void URenderSoundVisualization::RenderSoundVis(UProceduralMeshComponent* mesh, UImportedSoundWave* sound, TArray<FVector> meshVertices, TArray<FLinearColor> meshVertexColors, float time, float duration, int subDivisionsX, int subDivisionsY)
{

	float l_ItDuration = duration / subDivisionsX;
	TArray<FLinearColor> l_VertexColor;
	l_VertexColor.AddZeroed(meshVertexColors.Num());


	for (int l_i = 0; l_i < subDivisionsX; l_i++) {

		float l_Math = time + (l_i * l_ItDuration);
		float l_Amp = GetMaxAmpForDuration(sound, l_Math, l_ItDuration);
		TArray<float> l_Frequencies = GetFrequenciesForDuration(sound, l_Math, l_ItDuration);

		float l_AmpConv = subDivisionsY * l_Amp;

		for (int l_y = 0; l_y < subDivisionsY; l_y++) {
			int l_Index = GetIndexFromCoordinate(l_i, l_y, subDivisionsY);

			float l_Math2 = (float)l_y / subDivisionsY;
			int l_Sub = 0;
			if (l_y == 0)
				l_Sub = 0;
			else
				l_Sub = 1;

			//UE_LOG(LogTemp, Display, TEXT("Math 2 : %f"), (l_Frequencies.Num() - l_Sub) * l_Math2);
			//UE_LOG(LogTemp, Display, TEXT("Freq Num : %d"), l_Frequencies.Num());

			if (l_y < l_AmpConv) {
				l_VertexColor[l_Index] = FLinearColor(l_Amp, l_Frequencies[(l_Frequencies.Num()) * l_Math2] / 20000, 0);
			}
		}
	}

	mesh->UpdateMeshSection_LinearColor(0, meshVertices, TArray<FVector>(), TArray<FVector2D>(), l_VertexColor, TArray<FProcMeshTangent>());
}
