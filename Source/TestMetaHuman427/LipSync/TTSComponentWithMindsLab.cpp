// Fill out your copyright notice in the Description page of Project Settings.


#include "TTSComponentWithMindsLab.h"
#include "Sound/SoundBase.h"
#include "AudioDecompress.h"
#include "AudioDevice.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTTSComponentWithMindsLab::UTTSComponentWithMindsLab()
{
	PrimaryComponentTick.bCanEverTick = true;

	Http = &FHttpModule::Get();

	// ...
}

void UTTSComponentWithMindsLab::CallTTS(FString TextForSound)
{
	if (SucceedSoundSetting && LastTextForSound == TextForSound) {
		if (bIsAutoPlay) UGameplayStatics::PlaySound2D(this, LastSoundWave);
		if (TTSEvent.IsBound())	TTSEvent.Broadcast(LastSoundWave);
		return;
	}

	SucceedSoundSetting = false;
	LastTextForSound = TextForSound;
	TextForSound += TEXT(".");

	// neocomix & mindslab
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	Request->OnProcessRequestComplete().BindUObject(this, &UTTSComponentWithMindsLab::OnResponseReceived);
	Request->SetURL("https://api.maum.ai/tts/stream");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	JsonObject->SetStringField("apiId", "neocomix");
	JsonObject->SetStringField("apiKey", "328239e0cac840929309d5de7faa09d0");
	JsonObject->SetStringField("text", TextForSound);
	JsonObject->SetStringField("voiceName", "neocomix_lye");

	FString outputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&outputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	Request->SetContentAsString(outputString);
	Request->ProcessRequest();
}

void UTTSComponentWithMindsLab::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TArray < uint8 > rawFile = Response->GetContent();
	//SerializeWaveFile(rawFile, Response->GetContent().GetData(), Response->GetContent().Num(), 1, 44100);

	FWaveModInfo WaveInfo;
	if (WaveInfo.ReadWaveInfo(rawFile.GetData(), rawFile.Num())) {

		USoundWave* sw = NewObject<USoundWave>();

		if (!sw) {
			UE_LOG(LogTemp, Warning, TEXT("Null Sound Wave"));
			return;
		}

		int32 DurationDiv = *WaveInfo.pChannels * (*WaveInfo.pBitsPerSample / 8.f) * *WaveInfo.pSamplesPerSec;
		if (DurationDiv <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Null Too small than DurationDiv"));
			return;
		}

		sw->Duration = WaveInfo.SampleDataSize / DurationDiv;
		sw->SetSampleRate(*WaveInfo.pSamplesPerSec); // 프로퍼티가 클 수록 소리가 빨라짐.
		sw->NumChannels = *WaveInfo.pChannels;
		sw->RawPCMDataSize = WaveInfo.SampleDataSize;
		sw->RawPCMData = (uint8*)FMemory::Malloc(sw->RawPCMDataSize);
		FMemory::Memmove(sw->RawPCMData, rawFile.GetData(), rawFile.Num());
		sw->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
		sw->InvalidateCompressedData();

		sw->RawData.Lock(LOCK_READ_WRITE);
		void* LockedData = sw->RawData.Realloc(rawFile.Num());
		FMemory::Memcpy(LockedData, rawFile.GetData(), rawFile.Num());
		sw->RawData.Unlock();

		LastSoundWave = sw;
		SucceedSoundSetting = true;

		if (bIsAutoPlay) UGameplayStatics::PlaySound2D(this, sw);

		if(TTSEvent.IsBound())	TTSEvent.Broadcast(sw);
	}
}

