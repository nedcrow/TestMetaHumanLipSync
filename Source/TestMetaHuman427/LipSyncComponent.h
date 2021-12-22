// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OVRLipSyncFrame.h"
#include "LipSyncComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTMETAHUMAN427_API ULipSyncComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULipSyncComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* SoundBase;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (Tooltip = "LipSync Sequence to be played"))
	UOVRLipSyncFrameSequence* LipSyncSequence;

	bool DecompressSoundWave(USoundWave* SoundWave);

	UFUNCTION(BlueprintCallable)
	bool OVRLipSyncProcessSoundWaveAsset(const FAssetData& SoundWaveAsset, bool UseOfflineModel = false);
	UFUNCTION(BlueprintCallable)
	bool OVRLipSyncProcessSoundWaveBase(USoundBase* TargetSoundBase, bool UseOfflineModel = false);
	bool OVRLipSyncProcessSoundWave(USoundWave* TargetSoundWave, bool UseOfflineModel = false);
};
