// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CommonAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class TESTMETAHUMAN427_API UCommonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", Meta = (ClampMin = "0.0", ClampMax="1.0", Tooltip = "최대값은 1."))
	float IdleAnimIndex = 0;

	UFUNCTION()
	void SetIdleAnimIndex();
};
