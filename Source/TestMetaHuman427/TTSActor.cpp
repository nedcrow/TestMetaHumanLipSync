// Fill out your copyright notice in the Description page of Project Settings.


#include "TTSActor.h"
#include "Sound/SoundBase.h"
#include "AudioDecompress.h"
#include "AudioDevice.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATTSActor::ATTSActor(const class FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Http = &FHttpModule::Get();
}

// Called when the game starts or when spawned
void ATTSActor::BeginPlay()
{
	Super::BeginPlay();
	CallTTS();
	
}

// Called every frame
void ATTSActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATTSActor::CallTTS()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	// mindslab
	Request->OnProcessRequestComplete().BindUObject(this, &ATTSActor::OnResponseReceived);
	Request->SetURL("https://api.maum.ai/tts/stream");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	JsonObject->SetStringField("apiId", "neocomix");
	JsonObject->SetStringField("apiKey", "328239e0cac840929309d5de7faa09d0");
	JsonObject->SetStringField("text", L"나랏말쌈이 듕귁에 달아 이롭지 아니할씨, 세종대왕 만세");
	JsonObject->SetStringField("voiceName", "neocomix_lye");

	// test rest
	/*Request->SetURL("https://jsonplaceholder.typicode.com/posts");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	JsonObject->SetStringField("title", "Test");
	JsonObject->SetStringField("body", L"안녕 I am testing!");
	JsonObject->SetStringField("userId", "1");*/

	FString outputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&outputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	Request->SetContentAsString(outputString);
	
	Request->ProcessRequest();
}

void ATTSActor::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("HTTP bWasSuccessful : %s"), bWasSuccessful ? TEXT("Success") : TEXT("Fail"));
	
	TArray < uint8 > rawFile;
	SerializeWaveFile(rawFile, Response->GetContent().GetData(), Response->GetContent().Num(), 1, 44100);

	FWaveModInfo WaveInfo;
	UE_LOG(LogTemp, Warning, TEXT("rawFile Num: %d"), rawFile.Num());
	if (WaveInfo.ReadWaveInfo(rawFile.GetData(), rawFile.Num())) {
		USoundWave* sw = NewObject<USoundWave>(USoundWave::StaticClass());
		if (!sw) {
			UE_LOG(LogTemp, Warning, TEXT("Null Sound Wave"));
			return;
		}
		
		UE_LOG(LogTemp, Warning, TEXT("SoundWave Name: %s"), *sw->GetFName().ToString());

		int32 DurationDiv = *WaveInfo.pChannels * *WaveInfo.pBitsPerSample * *WaveInfo.pSamplesPerSec;
		if (DurationDiv <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Null Too small than DurationDiv"));			
		}
		
		sw->Duration = *WaveInfo.pWaveDataSize * 8.0f / DurationDiv;
		sw->SetSampleRate(*WaveInfo.pSamplesPerSec/2.0f);
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

		UGameplayStatics::PlaySound2D(this, sw);
	}
	
	//auto content = Response->GetContent().GetData();
	//FString contentStr = Response->GetContentAsString();
	//auto headers = Response->GetAllHeaders();
	//auto contentType = Response->GetContentType();
	//auto contentLength = Response->GetContentLength();
	////USoundWave* SW;
	////Create a pointer to hold the json serialized data
	//TSharedPtr<FJsonObject> JsonObject;

	////Create a reader pointer to read the json data
	//TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	////Deserialize the json data given Reader and the actual object to deserialize
	//if (FJsonSerializer::Deserialize(Reader, JsonObject))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("HTTP Response result : %s"), *Response->GetContentAsString());
	//}

}

