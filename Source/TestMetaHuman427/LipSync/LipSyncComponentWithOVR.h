// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OVRLipSyncFrame.h"
#include "LipSyncComponentWithOVR.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTMETAHUMAN427_API ULipSyncComponentWithOVR : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULipSyncComponentWithOVR();

/* Properties */
public:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (Tooltip = "Use when functions(OVRLipSyncProcess) has null sound property."))
	USoundBase* DefaultSoundBase;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (Tooltip = "LipSync Sequence to be played"))
	UOVRLipSyncFrameSequence* LipSyncSequence;

/*
* OVRLipSyncProcess
* Reference : OVRLipSyncEditorModule
*/
	UFUNCTION(BlueprintCallable)
	bool OVRLipSyncProcessSoundWaveAsset(const FAssetData& SoundWaveAsset, bool UseOfflineModel = false);
	UFUNCTION(BlueprintCallable)
	bool OVRLipSyncProcessSoundBase(USoundBase* TargetSoundBase, bool UseOfflineModel = false);
	UFUNCTION(BlueprintCallable)
	bool OVRLipSyncProcessSoundWave(USoundWave* TargetSoundWave, bool UseOfflineModel = false);

private:
	USoundWave* LastSoundWave;
	bool DecompressSoundWave(USoundWave* SoundWave);
};
