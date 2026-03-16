#include "CController.h"

void ACController::WidgetTimeSliderChanged(float newValue)
{
	SetTime(newValue);
}

void ACController::Play() {
	if (MapInfo->Song == nullptr) return;
	if (PlayingTime >= MapInfo->Song->Duration) return;

	Playing = true;

	if (PlayingTime < 0) {
		UE_LOG(LogTemp, Error, TEXT("Playing time got under 0 ???"));
		PlayingTime = 0;
	}

	StartedPlayTime = ActorTime - PlayingTime;
	AudioComponent->Sound = MapInfo->Song;
	AudioComponent->Play(PlayingTime);
}

void ACController::Stop() {
	if (!Playing) return;

	Playing = false;
	AudioComponent->Stop();
}

void ACController::AddTime(float p_Value) {
	if (BeatCells == nullptr || Playing) return;

	if (PlayingTime + p_Value < 0) {
		UE_LOG(LogTemp, Error, TEXT("[AddTime] Playing Time got under 0 ???"));
		PlayingTime = 0;
	} else if (PlayingTime + p_Value > SongDuration && p_Value > 0) {
		PlayingTime = SongDuration;
	}
	else {
		PlayingTime += p_Value;
	}

	UpdateBeatGrid();
	UpdateVisMesh();
}

void ACController::SetTime(float time)
{
	if (Playing) {
		Stop();
	}

	float l_Time = time;
	if (l_Time >= SongDuration)
		l_Time = SongDuration;

	if (l_Time < 0)
		l_Time = 0;

	PlayingTime = l_Time;
	if (l_Time <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("[SetTime] Playing Time got to 0 ???"));
	}
	UpdateBeatGrid();
	UpdateVisMesh();

	if (Playing) {
		Play();
	}
}
