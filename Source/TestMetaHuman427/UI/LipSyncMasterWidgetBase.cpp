// Fill out your copyright notice in the Description page of Project Settings.


#include "LipSyncMasterWidgetBase.h"
#include "CharacterListWidgetBase.h"
#include "AnimationButtonWidgetBase.h"
#include "AnimationListWidgetBase.h"
#include "../LipSyncModelMetaHuman.h"
#include "../LipSyncModelReallusion.h"

#include "Components/AudioComponent.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableText.h"
#include "Kismet/GameplayStatics.h"

void ULipSyncMasterWidgetBase::NativeConstruct() {
	/* Apply widgets */
	CharacterList = Cast<UCharacterListWidgetBase>(UUserWidget::GetWidgetFromName(TEXT("CharacterListWidget")));
	CharacterList->TargetLipSyncMasterWidget = this;

	AnimationList = Cast<UAnimationListWidgetBase>(UUserWidget::GetWidgetFromName(TEXT("AnimationListWidget")));

	InputButtonText = Cast<UTextBlock>(UUserWidget::GetWidgetFromName(TEXT("InputButtonText")));

	InputTextBox = Cast<UMultiLineEditableText>(UUserWidget::GetWidgetFromName(TEXT("InputTextBox")));

	HideUIButton = Cast<UButton>(UUserWidget::GetWidgetFromName(TEXT("HideUIButton")));
	HideUIButton->OnClicked.AddDynamic(this,&ULipSyncMasterWidgetBase::HideWidget);
	
	MainCanvasPanel = Cast<UCanvasPanel>(UUserWidget::GetWidgetFromName(TEXT("MainCanvasPanel")));

	/* bind events */
	ALipSyncModelMetaHuman* model_metahuman = Cast<ALipSyncModelMetaHuman>(UGameplayStatics::GetActorOfClass(GetWorld(), ALipSyncModelMetaHuman::StaticClass()));
	ALipSyncModelReallusion* model_reallusion = Cast<ALipSyncModelReallusion>(UGameplayStatics::GetActorOfClass(GetWorld(), ALipSyncModelReallusion::StaticClass()));

	if (!(model_metahuman && model_reallusion)) return;

	model_metahuman->Audio->OnAudioFinished.AddDynamic(this, &ULipSyncMasterWidgetBase::UnlockingWidge);
	model_reallusion->Audio->OnAudioFinished.AddDynamic(this, &ULipSyncMasterWidgetBase::UnlockingWidge);
}

void ULipSyncMasterWidgetBase::UnlockingWidge() {
	InputButtonText->SetText(FText().FromString(TEXT("더빙")));
	if (MainCanvasPanel) MainCanvasPanel->SetVisibility(ESlateVisibility::Visible);
}

void ULipSyncMasterWidgetBase::LockingWidge() {
	InputButtonText->SetText(FText().FromString(TEXT("ing")));
	if (MainCanvasPanel) MainCanvasPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void ULipSyncMasterWidgetBase::BLockingWidget()
{
	LockingWidge();
}

void ULipSyncMasterWidgetBase::HideWidget()
{
	if (MainCanvasPanel) {
		MainCanvasPanel->SetVisibility(ESlateVisibility::Hidden); 
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Failed the HideWidget"));
	}
}

void ULipSyncMasterWidgetBase::UnHideWidget()
{
	if (MainCanvasPanel) {
		MainCanvasPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Failed the UnHideWidget"));
	}
}

void ULipSyncMasterWidgetBase::CallLastAnimation()
{
	UAnimationButtonWidgetBase* button = Cast<UAnimationButtonWidgetBase>(AnimationList->ButtonList->GetChildAt(AnimationList->LastAnimButtonIndex));
	if (button) button->OnClickedAnimationButton();
	
}
