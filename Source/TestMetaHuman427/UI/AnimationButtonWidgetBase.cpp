// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationButtonWidgetBase.h"
#include "AnimationListWidgetBase.h"
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

	UWorld* world = GetWorld();
	if (!world) return;


	/* 캐릭터 애니메이션 변경 */
	FString tempStr = TargetText->Text.ToString();
	if (model->CurrentAnimation == tempStr) {
		if (tempStr == TEXT("SittingIdle")) { model->CurrentAnimation = TEXT("SittingTalking"); }
		else if (tempStr == TEXT("Idle")) { model->CurrentAnimation = TEXT("Thinking"); }
		else { model->CurrentAnimation = tempStr.Mid(0, 3) == TEXT("Sit") ? TEXT("SittingIdle") : TEXT("Idle"); }

		world->GetTimerManager().SetTimer(AnimationButtonTimer, this, &UAnimationButtonWidgetBase::OnClickedAnimationButton, 0.01f, false);
		return;
	}
	model->CurrentAnimation = TargetText->Text.ToString();
	if (ParentList) ParentList->LastAnimButtonIndex = IndexOfList;

	/* Camera Z 위치 보정 */
	world->GetTimerManager().SetTimer(AnimationButtonTimer, this, &UAnimationButtonWidgetBase::LockCameraZ, 0.06f, false);
}

void UAnimationButtonWidgetBase::LockCameraZ() {
	ALipSyncModelMetaHuman* model = Cast<ALipSyncModelMetaHuman>(UGameplayStatics::GetActorOfClass(GetWorld(), ALipSyncModelMetaHuman::StaticClass()));
	ALipSyncCameraPawn* cameraPawn = Cast<ALipSyncCameraPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!(model->Body && cameraPawn)) return;

	FVector CameraSocketLocation = model->Body->GetSocketLocation(TEXT("CameraSocket"));
	cameraPawn->LockZLocation(CameraSocketLocation.Z);
}