// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationButtonWidgetBase.h"
#include "../LipSyncModelMetaHuman.h"
#include "../LipSyncCameraPawn.h"

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

void UAnimationButtonWidgetBase::ApplyButtonText(FString Text)
{
	if (TargetText) TargetText->SetText(FText::FromString(Text));
}

void UAnimationButtonWidgetBase::OnClickedAnimationButton()
{
	ALipSyncModelMetaHuman* model = Cast<ALipSyncModelMetaHuman>(UGameplayStatics::GetActorOfClass(GetWorld(), ALipSyncModelMetaHuman::StaticClass()));
	if (!model || !TargetAnimation) return;

	/* 캐릭터 애니메이션 변경 */
	model->CurrentAnimation = TargetText->Text.ToString();
	model->Body->PlayAnimation(TargetAnimation, bIsLoopTargetAnim);

	/* Camera Z 위치 보정 */
	UWorld* world = GetWorld();
	if (!world) return;
	world->GetTimerManager().SetTimer(CameraTimer, this, &UAnimationButtonWidgetBase::LockCameraZ, 0.08f, false);
}

void UAnimationButtonWidgetBase::LockCameraZ() {
	ALipSyncModelMetaHuman* model = Cast<ALipSyncModelMetaHuman>(UGameplayStatics::GetActorOfClass(GetWorld(), ALipSyncModelMetaHuman::StaticClass()));
	ALipSyncCameraPawn* cameraPawn = Cast<ALipSyncCameraPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!(model->Body && cameraPawn)) return;

	FVector CameraSocketLocation = model->Body->GetSocketLocation(TEXT("CameraSocket"));
	cameraPawn->LockZLocation(CameraSocketLocation.Z);
}