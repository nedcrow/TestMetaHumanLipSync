// Fill out your copyright notice in the Description page of Project Settings.


#include "LipSyncComponentWithOVR.h"
#include "Engine.h"
#include "AudioDecompress.h"
#include "AudioDevice.h"
#include "OVRLipSyncContextWrapper.h"
#include "OVRLipSyncFrame.h"

// Sets default values for this component's properties
ULipSyncComponentWithOVR::ULipSyncComponentWithOVR()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool ULipSyncComponentWithOVR::OVRLipSyncProcessSoundWaveAsset(const FAssetData& SoundWaveAsset, bool UseOfflineModel)
{
	auto ObjectPath = SoundWaveAsset.ObjectPath.ToString();
	auto SoundWave = FindObject<USoundWave>(NULL, *ObjectPath);
	bool isFail = false;

	if (!SoundWave)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find %s"), *ObjectPath);
		isFail = true;
	}

	if (SoundWave->NumChannels > 2)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't process %s: only mono and stereo streams are supported"), *ObjectPath);
		isFail = true;
	}

	if (isFail) {
		if (DefaultSoundBase) {
			OVRLipSyncProcessSoundBase(DefaultSoundBase);
			return true;
		}
		return false;
	}

	OVRLipSyncProcessSoundWave(SoundWave);
	return true;
}

bool ULipSyncComponentWithOVR::OVRLipSyncProcessSoundBase(USoundBase* TargetSoundBase, bool UseOfflineModel)
{
	if (!TargetSoundBase)
	{
		UE_LOG(LogTemp, Error, TEXT("Null property 'USoundBase'."));
		
		if (DefaultSoundBase) {
			OVRLipSyncProcessSoundBase(DefaultSoundBase);
			return true;
		}
		return false;
	}

	USoundWave* SoundWave = Cast<USoundWave>(TargetSoundBase);
	OVRLipSyncProcessSoundWave(SoundWave);
	return true;
}

bool ULipSyncComponentWithOVR::OVRLipSyncProcessSoundWave(USoundWave* TargetSoundWave, bool UseOfflineModel)
{
	if (!TargetSoundWave) {
		UE_LOG(LogTemp, Error, TEXT("Null property 'USoundWave'."));

		if (!DefaultSoundBase) {
			UE_LOG(LogTemp, Error, TEXT("Null property 'DefaultSoundBase'."));
			return false;
		}

		OVRLipSyncProcessSoundBase(DefaultSoundBase);
		return true;
	}

	if (LastSoundWave == TargetSoundWave) return true;
	LastSoundWave = TargetSoundWave;

	DecompressSoundWave(TargetSoundWave);

	// Compute LipSync sequence frames at 100 times a second rate
	constexpr auto LipSyncSequenceUpateFrequency = 100;
	constexpr auto LipSyncSequenceDuration = 1.0f / LipSyncSequenceUpateFrequency;

	auto SequenceName = FString::Printf(TEXT("%s_LipSyncSequence"), *TargetSoundWave->GetFName().ToString());
	auto SequencePath = FString::Printf(TEXT("/Game/Audio/TempSound_LipSyncSequence"));
	auto SequencePackage = CreatePackage(*SequencePath);
	auto Sequence = NewObject<UOVRLipSyncFrameSequence>(SequencePackage, *SequenceName, RF_Public | RF_Standalone);
	auto NumChannels = TargetSoundWave->NumChannels;
	auto SampleRate = TargetSoundWave->GetSampleRateForCurrentPlatform();
	auto PCMDataSize = TargetSoundWave->RawPCMDataSize / sizeof(int16_t);
	auto PCMData = reinterpret_cast<int16_t*>(TargetSoundWave->RawPCMData);
	auto ChunkSizeSamples = static_cast<int>(SampleRate * LipSyncSequenceDuration);
	auto ChunkSize = NumChannels * ChunkSizeSamples;

	FString ModelPath = UseOfflineModel ? FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("OVRLipSync"),
		TEXT("OfflineModel"), TEXT("ovrlipsync_offline_model.pb"))
		: FString();
	UOVRLipSyncContextWrapper context(ovrLipSyncContextProvider_Enhanced, SampleRate, 4096, ModelPath);

	float LaughterScore = 0.0f;
	int32_t FrameDelayInMs = 0;
	TArray<float> Visemes;

	TArray<int16_t> samples;
	samples.SetNumZeroed(ChunkSize);
	context.ProcessFrame(samples.GetData(), ChunkSizeSamples, Visemes, LaughterScore, FrameDelayInMs, NumChannels > 1);

	int FrameOffset = (int)(FrameDelayInMs * SampleRate / 1000 * NumChannels);

	FScopedSlowTask SlowTask(PCMDataSize + FrameOffset,
		FText::Format(NSLOCTEXT("NSLT_OVRLipSyncPlugin", "GeneratingLipSyncSequence",
			"Generating LipSync sequence for {0}..."),
			FText::FromName(TargetSoundWave->GetFName())));
	SlowTask.MakeDialog();
	for (int offs = 0; offs < PCMDataSize + FrameOffset; offs += ChunkSize)
	{
		int remainingSamples = PCMDataSize - offs;
		if (remainingSamples >= ChunkSize)
		{
			context.ProcessFrame(PCMData + offs, ChunkSizeSamples, Visemes, LaughterScore, FrameDelayInMs,
				NumChannels > 1);
		}
		else
		{
			if (remainingSamples > 0)
			{
				memcpy(samples.GetData(), PCMData + offs, sizeof(int16_t) * remainingSamples);
				memset(samples.GetData() + remainingSamples, 0, ChunkSize - remainingSamples);
			}
			else
			{
				memset(samples.GetData(), 0, ChunkSize);
			}
			context.ProcessFrame(samples.GetData(), ChunkSizeSamples, Visemes, LaughterScore, FrameDelayInMs,
				NumChannels > 1);
		}

		SlowTask.EnterProgressFrame(ChunkSize);
		if (SlowTask.ShouldCancel())
		{
			return false;
		}
		if (offs >= FrameOffset)
		{
			Sequence->Add(Visemes, LaughterScore);
		}
	}
	
	LipSyncSequence = Sequence;

	Sequence->MarkPackageDirty();

	return true;
}

bool ULipSyncComponentWithOVR::DecompressSoundWave(USoundWave* SoundWave)
{
	if (SoundWave->RawPCMData)
	{
		return true;
	}
	auto AudioDevice = GEngine->GetMainAudioDevice();
	if (!AudioDevice)
	{
		return false;
	}

	AudioDevice->StopAllSounds(true);
	auto OriginalDecompressionType = SoundWave->DecompressionType;
	SoundWave->DecompressionType = DTYPE_Native;
	if (SoundWave->InitAudioResource(AudioDevice->GetRuntimeFormat(SoundWave)))
	{
		USoundWave::FAsyncAudioDecompress Decompress(SoundWave, MONO_PCM_BUFFER_SAMPLES);
		Decompress.StartSynchronousTask();
	}
	SoundWave->DecompressionType = OriginalDecompressionType;

	return true;
}
