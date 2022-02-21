// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LipSync/LipSyncModelBase.h"
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

};
