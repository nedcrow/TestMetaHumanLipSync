// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "TTSComponentWithMindsLab.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynimic_OneParam, USoundWave*, SomeParameter);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTMETAHUMAN427_API UTTSComponentWithMindsLab : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTTSComponentWithMindsLab();

public:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data | Option")
	bool bIsAutoPlay = false;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Data | Event")
	FDele_Dynimic_OneParam TTSEvent;

	FHttpModule* Http;

	FString DialogStr;

	USoundBase* SoundBase;

	UFUNCTION(BlueprintCallable)
	void CallTTS(FString TextForSound);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
		
};
