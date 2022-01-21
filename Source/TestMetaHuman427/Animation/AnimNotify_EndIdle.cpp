// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_EndIdle.h"
#include "CommonAnimInstance.h"

void UAnimNotify_EndIdle::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	// ��� �ִϸ��̼� �缳��
	UCommonAnimInstance* animInstance = Cast<UCommonAnimInstance>(MeshComp->GetAnimInstance());
	if (animInstance) {
		animInstance->SetIdleAnimIndex();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Null animinstance"));
	}

}