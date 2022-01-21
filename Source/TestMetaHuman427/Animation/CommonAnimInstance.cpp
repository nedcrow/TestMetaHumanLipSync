// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonAnimInstance.h"

void UCommonAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	
}

void UCommonAnimInstance::SetIdleAnimIndex()
{
	IdleAnimIndex = FMath::RandRange(0.0f, 1.0f);
}
