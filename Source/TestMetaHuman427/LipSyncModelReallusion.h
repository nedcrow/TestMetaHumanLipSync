// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LipSyncModelBase.h"
#include "LipSyncModelReallusion.generated.h"

/**
 * 
 */
UCLASS()
class TESTMETAHUMAN427_API ALipSyncModelReallusion : public ALipSyncModelBase
{
	GENERATED_BODY()

public:
	ALipSyncModelReallusion();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
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
	uint8 bUseLookAtCursor: 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data | LookAt")
	uint8 bTriggerd:1;

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
