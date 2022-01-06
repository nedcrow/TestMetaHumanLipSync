// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LipSyncModelBase.generated.h"

UCLASS()
class TESTMETAHUMAN427_API ALipSyncModelBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALipSyncModelBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USkeletalMeshComponent* Body;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UOVRLipSyncPlaybackActorComponent* OVRLipSyncPlaybackActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ULipSyncComponentWithOVR* LipSyncWithOVR;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTTSComponentWithMindsLab* TTSWithMindsLab;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USceneCaptureComponent2D* SceneCapture2D;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UAudioComponent* Audio;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

// OVRLipSync
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data")
	USkeletalMeshComponent* LipSyncTargetSkeletalMesh;

	UFUNCTION()
	void CallDeleFunc_VisemesReady();

	UFUNCTION(BlueprintCallable)
	void StartLipSync(USoundBase* SoundBase);

// LipSync_TTS
public:
	UFUNCTION()
	void CallDeleFunc_TTSLipSync(USoundWave* TargetSoundWave);

	UFUNCTION(BlueprintCallable)
	void StartLipSyncFromText(FString TargetText);
	
};
