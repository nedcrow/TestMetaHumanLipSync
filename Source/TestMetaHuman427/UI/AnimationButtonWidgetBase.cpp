// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationButtonWidgetBase.h"
#include "../LipSyncModelMetaHuman.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"

void UAnimationButtonWidgetBase::NativeConstruct()
{
	TargetButton = Cast<UButton>(UUserWidget::GetWidgetFromName(TEXT("TargetButton")));
	if (TargetButton) TargetButton->OnClicked.AddDynamic(this, &UAnimationButtonWidgetBase::OnClickedAnimationButton);

	TargetText = Cast<UTextBlock>(UUserWidget::GetWidgetFromName(TEXT("TargetText")));
}

void UAnimationButtonWidgetBase::OnClickedAnimationButton()
{
	FStreamableManager loader;
	if (!TargetAnimation) {
		UE_LOG(LogTemp, Warning, TEXT("Null Animation"));
		return;
	}

	/* 캐릭터 애니메이션 변경 */
	ALipSyncModelMetaHuman* model = Cast<ALipSyncModelMetaHuman>(UGameplayStatics::GetActorOfClass(GetWorld(), ALipSyncModelMetaHuman::StaticClass()));
	if (!model) {
		UE_LOG(LogTemp, Warning, TEXT("Null Model"));
		return;
	}

	model->Body->PlayAnimation(TargetAnimation, bIsLoopTargetAnim);
}