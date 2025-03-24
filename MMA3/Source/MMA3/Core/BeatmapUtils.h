// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class MMA3_API BeatmapUtils
{
public:
	BeatmapUtils();
	~BeatmapUtils();

	static float CalculateBeatCount(float p_SongDuration, float p_BPM) {
		return (p_SongDuration / 60.f) * p_BPM;
	}

};
