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
};
