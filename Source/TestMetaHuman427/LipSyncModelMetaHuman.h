// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LipSyncModelBase.h"
#include "LipSyncModelMetaHuman.generated.h"

/**
 * 
 */
UCLASS()
class TESTMETAHUMAN427_API ALipSyncModelMetaHuman : public ALipSyncModelBase
{
	GENERATED_BODY()
public:
	ALipSyncModelMetaHuman();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USkeletalMeshComponent* Face;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UGroomComponent* Hair;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UGroomComponent* Eyebrows;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UGroomComponent* Fuzz;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UGroomComponent* Eyelashes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UGroomComponent* Mustache;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UGroomComponent* Bread;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USkeletalMeshComponent* Legs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USkeletalMeshComponent* Feet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USkeletalMeshComponent* Torso;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

// LookAtCursor
public:
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
