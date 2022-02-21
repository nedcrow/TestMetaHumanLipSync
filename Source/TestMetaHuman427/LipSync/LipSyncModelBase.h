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
	class USceneComponent* Root;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USkeletalMeshComponent* Body;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USphereComponent* CameraTarget;

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

	bool isPlayed = false;

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

// Animation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data | Animation")
	FString CurrentAnimation;

// LookAtCamera
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data | LookAt")
	uint8 bUseLookAtCamera : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data | LookAt")
	FVector TargetCameraLocation = FVector(-150.0f, .0f, 150.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data | LookAt")
	FVector CameraLocation_Min = FVector(-150.0f, -70.0f, 120.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data | LookAt")
	FVector CameraLocation_Max = FVector(0.0f, 70.0f, 240.0f);

	UFUNCTION(BlueprintCallable)
	void SetCameraLocation();

// LookAtCursor
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data | LookAt")
	uint8 bUseLookAtCursor : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data | LookAt")
	uint8 bTriggerd : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data | LookAt")
	FVector CursorLocationHead = FVector(150.0f, .0f, 150.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data | LookAt")
	FVector CursorLocationEyes = FVector(150.0f, .0f, 160.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data | LookAt")
	FVector OutLocationHead;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data | LookAt")
	float RotationSpeed = 10.f;

	UFUNCTION(BlueprintCallable)
	void SetCursorLocations();
};
