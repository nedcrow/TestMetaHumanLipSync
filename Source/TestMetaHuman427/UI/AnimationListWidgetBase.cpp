// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimationListWidgetBase.h"
#include "AnimationButtonWidgetBase.h"
#include "LipSyncMasterWidgetBase.h"
#include "../DataStruct.h"

#include "Components/ScrollBox.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"

void UAnimationListWidgetBase::NativeConstruct() {
	ButtonList = Cast<UScrollBox>(UUserWidget::GetWidgetFromName(TEXT("ButtonList")));

	InitAnimationButtons(0);
}

void UAnimationListWidgetBase::InitButtonList(int ButtonCount)
{
	Super::InitButtonList(ButtonCount);
}

void UAnimationListWidgetBase::InitAnimationButtons(int TargetRowIndex)
{
	if (!TargetDataTable->IsValidLowLevel()) {
		UE_LOG(LogTemp, Warning, TEXT("Null TargetDataTable"));
		return;
	}

	if (ButtonList) {
		FName rowName = FName(*FString().FromInt(TargetRowIndex));
		FAnimationIconStruct* rowData = TargetDataTable->FindRow<FAnimationIconStruct>(rowName, TEXT(""));

		/* 필요한 버튼 준비 */
		InitButtonList(rowData->Animations.Num());

		/* 각 버튼마다 animation, 이름, 인덱스, 부모 리스트 할당 */
		FStreamableManager loader;
		for (int i = 0; i < ButtonList->GetChildrenCount(); i++) {
			UAnimationButtonWidgetBase* AnimButtonWidget = Cast<UAnimationButtonWidgetBase>(ButtonList->GetChildAt(i));
			UAnimationAsset* anim = loader.LoadSynchronous<UAnimationAsset>(rowData->Animations[i]);

			if (!(anim && AnimButtonWidget)) return;
			AnimButtonWidget->TargetAnimation = anim;
			AnimButtonWidget->bIsLoopTargetAnim = i == 0 ? true : false;

			AnimButtonWidget->ApplyButtonText(rowData->AnimNames[i]);

			AnimButtonWidget->IndexOfList = i;
			AnimButtonWidget->ParentList = this;
		}
	}
}
