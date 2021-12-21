// Fill out your copyright notice in the Description page of Project Settings.


#include "TTSActor.h"
#include "Sound/SoundBase.h"

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

	Request->OnProcessRequestComplete().BindUObject(this, &ATTSActor::OnResponseReceived);
	////This is the url on which to process the request
	//Request->SetURL("https://api.maum.ai/tts/stream");
	//Request->SetVerb("POST");
	////Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	//Request->SetHeader("Content-Type", TEXT("application/json"));
	//DialogStr = TEXT("'나랏말쌈이 듕귁에 달아 이롭지 아니할씨, 세종대왕 만세'");
	//FString content = TEXT("{");
	//content.Append(TEXT("apiId : 'neocomix',"));
	//content.Append(TEXT("apiKey : '328239e0cac840929309d5de7faa09d0',"));
	//content.Append(TEXT("text : ")).Append(DialogStr).Append(TEXT(","));
	//content.Append(TEXT("voiceName : 'neocomix_lye'"));
	//content.Append(TEXT("}"));
	//UE_LOG(LogTemp, Warning, TEXT("testText: %s"), *content);
	//Request->SetContentAsString(content);
	//Request->ProcessRequest();

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetStringField("title", "Test");
	JsonObject->SetStringField("body", "I am testing!");
	JsonObject->SetStringField("userId", "1");

	Request->SetURL("https://jsonplaceholder.typicode.com/posts");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	FString content = TEXT("{");
	content.Append(TEXT("title: 'Test'"));
	content.Append(TEXT("body: 'I am testing!'"));
	content.Append(TEXT("userId: '1'"));
	content.Append(TEXT("}"));
	//Request->SetContentAsString(content);

	FString outputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&outputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	Request->SetContentAsString(outputString);
	Request->ProcessRequest();
}

void ATTSActor::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("HTTP bWasSuccessful : %s"), bWasSuccessful ? TEXT("Success") : TEXT("Fail"));

	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		//Get the value of the json object by field name
		//int32 recievedInt = JsonObject->GetIntegerField("customInt");

		//Output it to the engine
		//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::FromInt(recievedInt));

		UE_LOG(LogTemp, Warning, TEXT("HTTP Response result : %s"), *Response->GetContentAsString());
	}

}

