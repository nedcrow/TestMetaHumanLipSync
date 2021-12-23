// Fill out your copyright notice in the Description page of Project Settings.


#include "LipSyncComponent.h"
#include "Engine.h"
#include "AudioDecompress.h"
#include "AudioDevice.h"
#include "OVRLipSyncContextWrapper.h"
#include "OVRLipSyncFrame.h"

// Sets default values for this component's properties
ULipSyncComponent::ULipSyncComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



// Called when the game starts
void ULipSyncComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//FOVRLipSyncEditorModule::d
}


// Called every frame
void ULipSyncComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Decompresses SoundWave object by initializing RawPCM data
bool ULipSyncComponent::DecompressSoundWave(USoundWave* SoundWave)
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

bool ULipSyncComponent::OVRLipSyncProcessSoundWaveAsset(const FAssetData& SoundWaveAsset, bool UseOfflineModel)
{
	auto ObjectPath = SoundWaveAsset.ObjectPath.ToString();
	auto SoundWave = FindObject<USoundWave>(NULL, *ObjectPath);
	if (!SoundWave)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find %s"), *ObjectPath);
		return false;
	}
	if (SoundWave->NumChannels > 2)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't process %s: only mono and stereo streams are supported"), *ObjectPath);
		return false;
	}
	DecompressSoundWave(SoundWave);

	OVRLipSyncProcessSoundWave(SoundWave);
	return true;
}

bool ULipSyncComponent::OVRLipSyncProcessSoundWaveBase(USoundBase* TargetSoundBase, bool UseOfflineModel)
{
	if (!TargetSoundBase)
	{
		UE_LOG(LogTemp, Error, TEXT("Null property 'USoundBase'."));
		return false;
	}

	USoundWave* SoundWave = Cast<USoundWave>(TargetSoundBase);
	if(SoundWave) DecompressSoundWave(SoundWave);

	OVRLipSyncProcessSoundWave(SoundWave);
	return true;
}

bool ULipSyncComponent::OVRLipSyncProcessSoundWave(USoundWave * TargetSoundWave, bool UseOfflineModel)
{
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

	//FAssetRegistryModule::AssetCreated(Sequence);
	Sequence->MarkPackageDirty();

	return true;
}