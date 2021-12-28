// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "TTSComponentWithMindsLab.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynimic_OneParam, USoundWave*, SoundParameter);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTMETAHUMAN427_API UTTSComponentWithMindsLab : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTTSComponentWithMindsLab();

/* REST API */
public:	
	FHttpModule* Http;
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

/* Minds Lab TTS API */
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data | Option")
	bool bIsAutoPlay = false;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Data | Event")
	FDele_Dynimic_OneParam TTSEvent;

	UFUNCTION(BlueprintCallable, Meta = (Tooltip = "If response is successful, call TTSEvent of this component."))
	void CallTTS(FString TextForSound);

private:
	FString LastTextForSound;
	USoundWave* LastSoundWave;
	bool SucceedSoundSetting = false;
};
