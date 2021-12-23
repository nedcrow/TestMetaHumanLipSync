// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "TTSComponentWithMindsLab.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDele_Multi_One, USoundWave*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTMETAHUMAN427_API UTTSComponentWithMindsLab : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTTSComponentWithMindsLab();

public:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data | Option")
	bool bIsAutoPlay = false;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data | Event")
	FDele_Multi_One TTSEvent;

	FHttpModule* Http;

	FString DialogStr;

	USoundBase* SoundBase;

	UFUNCTION()
	void CallTTS(FString TextForSound);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
		
};
